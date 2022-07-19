#include "MemoryReaderAsyncWorker.h"

Napi::Value runMemoryReaderAsyncWorker(const CallbackInfo &info)
{
    Napi::Env env = info.Env();

    std::string windowTitle = info[0].ToString().Utf8Value();
    std::string regexString = info[1].ToString().Utf8Value();
    
    MemoryReaderAsyncWorker *asyncWorker = new MemoryReaderAsyncWorker(env, windowTitle, regexString);

    auto promise = asyncWorker->GetPromise();

    asyncWorker->Queue();

    return promise;
};


Object Init(Env env, Object exports)
{
    exports["sync"] = Function::New(
        env, runMemoryReaderAsyncWorker, std::string("runMemoryReaderAsyncWorker"));
    return exports;
}

NODE_API_MODULE(addon, Init)