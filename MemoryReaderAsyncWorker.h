#pragma once
#include <napi.h>
using namespace Napi;

class MemoryReaderAsyncWorker : public AsyncWorker
{
public:
    MemoryReaderAsyncWorker(std::string windowTitle, std::string regexString, Function &callback);
    virtual ~MemoryReaderAsyncWorker(){};

    void Execute();
    void OnOK();

private:
    std::string windowTitle;
    std::string regexString;
    const char* returnValue;
};