#include "MemoryReaderAsyncWorker.h"
#include <chrono>
#include <thread>
#include <psapi.h>
#include <iostream>
#include <vector>
#include <regex>
#include <algorithm>
#include <napi.h>
#include <windows.h>

MemoryReaderAsyncWorker::MemoryReaderAsyncWorker(std::string windowTitle, std::string regexString, Function &callback)
    : AsyncWorker(callback), regexString(regexString), windowTitle(windowTitle){};

void MemoryReaderAsyncWorker::Execute()
{
    std::regex regex(this->regexString);
    std::vector<char> buffer;
    DWORD pID;
    HWND hwnd = FindWindowA(NULL, this->windowTitle.c_str());
    if (!hwnd)
    {
        this->returnValue = "FindWindowA ERROR";
        return;
    }
    if (!GetWindowThreadProcessId(hwnd, &pID))
    {
        this->returnValue = "GetWindowThreadProcessId ERROR";
        return;
    }
    HANDLE pHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pID);
    if (!pHandle)
    {
        this->returnValue = "OpenProcess ERROR";
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
                
                this->returnValue = _strdup(MATCH.c_str());
                return;
            }
        }
    }

    this->returnValue = "MATCH NOT FOUND";
    return;
};

void MemoryReaderAsyncWorker::OnOK()
{
    Callback().Call({Env().Null(), String::New(Env(), this->returnValue)});
};