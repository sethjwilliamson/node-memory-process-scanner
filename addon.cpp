#include "MemoryReaderAsyncWorker.h"

Value runMemoryReaderAsyncWorker(const CallbackInfo &info)
{
    std::string windowTitle = info[0].ToString().Utf8Value();
    std::string regexString = info[1].ToString().Utf8Value();
    Function callback = info[2].As<Function>();
    MemoryReaderAsyncWorker *asyncWorker = new MemoryReaderAsyncWorker(windowTitle, regexString, callback);
    asyncWorker->Queue();
    std::string msg = "Test1";
    return String::New(info.Env(), msg.c_str());
};

Object Init(Env env, Object exports)
{
    exports["sync"] = Function::New(
        env, runMemoryReaderAsyncWorker, std::string("runMemoryReaderAsyncWorker"));
    return exports;
}

NODE_API_MODULE(addon, Init)