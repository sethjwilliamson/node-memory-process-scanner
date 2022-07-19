#include "MemoryReaderAsyncWorker.h"

Napi::Value runMemoryReaderAsyncWorker(const CallbackInfo &info)
{
    Napi::Env env = info.Env();

    std::string windowTitle = info[0].ToString().Utf8Value();
    std::string regexString = info[1].ToString().Utf8Value();
    
    Napi::Promise::Deferred deferred = Napi::Promise::Deferred::New(env);
    
    MemoryReaderAsyncWorker *asyncWorker = new MemoryReaderAsyncWorker(env, windowTitle, regexString, deferred);
    asyncWorker->Queue();

    return deferred.Promise();
};


Object Init(Env env, Object exports)
{
    exports["scan"] = Function::New(
        env, runMemoryReaderAsyncWorker, std::string("runMemoryReaderAsyncWorker"));
    return exports;
}

NODE_API_MODULE(addon, Init)