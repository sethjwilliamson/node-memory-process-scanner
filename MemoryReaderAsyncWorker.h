#pragma once
#include <napi.h>
using namespace Napi;

class MemoryReaderAsyncWorker : public AsyncWorker
{
public:
    MemoryReaderAsyncWorker(Napi::Env &env, std::string windowTitle, std::string regexString);
    ~MemoryReaderAsyncWorker(){};

    void Execute();
    void OnOK();
    void OnError(Napi::Error const &error);
    Napi::Promise GetPromise();

private:
    std::string windowTitle;
    std::string regexString;
    Napi::Promise::Deferred deferred;
    const char* returnValue;
};