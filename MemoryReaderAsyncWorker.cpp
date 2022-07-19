#include "MemoryReaderAsyncWorker.h"
#include <regex>
#include <napi.h>
#include <windows.h>

MemoryReaderAsyncWorker::MemoryReaderAsyncWorker(Napi::Env &env, std::string windowTitle, std::string regexString)
    : Napi::AsyncWorker(env), regexString(regexString), windowTitle(windowTitle), deferred(Napi::Promise::Deferred::New(env)){};

void MemoryReaderAsyncWorker::Execute()
{
    std::regex regex(regexString);
    std::vector<char> buffer;
    DWORD pID;
    HWND hwnd = FindWindowA(NULL, windowTitle.c_str());
    if (!hwnd)
    {
        returnValue = "FindWindowA ERROR";
        return;
    }
    if (!GetWindowThreadProcessId(hwnd, &pID))
    {
        returnValue = "GetWindowThreadProcessId ERROR";
        return;
    }
    HANDLE pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);
    if (!pHandle)
    {
        returnValue = "OpenProcess ERROR";
        return;
    }
    unsigned char *p = NULL;
    MEMORY_BASIC_INFORMATION info;

    for (p; VirtualQueryEx(pHandle, p, &info, sizeof(info)) == sizeof(info); p += info.RegionSize)
    {
        if (info.State == MEM_COMMIT && (info.Type == MEM_MAPPED || info.Type == MEM_PRIVATE))
        {
            SIZE_T bytes_read;
            buffer.resize(info.RegionSize);

            ReadProcessMemory(pHandle, p, &buffer[0], info.RegionSize, &bytes_read);

            buffer.resize(bytes_read);
            std::string string = std::string(buffer.data(), buffer.size());

            string.erase(
                remove_if(string.begin(), string.end(), [](char c)
                            { return !(c >= 32 && c < 126); }),
                string.end());

            std::sregex_iterator it(string.begin(), string.end(), regex);
            std::sregex_iterator end;
            for (; it != end; ++it)
            {
                std::smatch match = *it;
                std::string MATCH = match[1].str();

                returnValue = _strdup(MATCH.c_str());
                return;
            }
        }
    }

    returnValue = "MATCH NOT FOUND";
    return;
};

void MemoryReaderAsyncWorker::OnOK()
{
    deferred.Resolve(Napi::String::New(Env(), returnValue));
};

void MemoryReaderAsyncWorker::OnError(Napi::Error const &error)
{
    deferred.Reject(error.Value());
};

Napi::Promise MemoryReaderAsyncWorker::GetPromise() { return deferred.Promise(); }

std::string windowTitle;
std::string regexString;
Napi::Promise::Deferred deferred;
const char *returnValue;