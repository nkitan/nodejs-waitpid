#include <node.h>
#include <v8.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <nan.h>
#include <errno.h>

using namespace v8;
using namespace node;

void Waitpid(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();
    int r, status;

    // check arguments
    if (args.Length() < 2) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Two arguments are required, PID and options", v8::NewStringType::kNormal).ToLocalChecked()));
        return;
    }
    if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "PID and options must be numbers", v8::NewStringType::kNormal).ToLocalChecked()));
        return;
    }

    // do the waitpid call
    r = waitpid(args[0]->NumberValue(Nan::GetCurrentContext()).ToChecked(), &status, args[1]->NumberValue(Nan::GetCurrentContext()).ToChecked());

    // return an object
    Local<Object> result = Object::New(isolate);
    Nan::Set(result, String::NewFromUtf8(isolate, "return", v8::NewStringType::kNormal).ToLocalChecked(), Number::New(isolate, r));

    if (WIFEXITED(status)) {
        Nan::Set(result, String::NewFromUtf8(isolate, "exitCode", v8::NewStringType::kNormal).ToLocalChecked(), Number::New(isolate, WEXITSTATUS(status)));
        Nan::Set(result, String::NewFromUtf8(isolate, "signalCode", v8::NewStringType::kNormal).ToLocalChecked(), Null(isolate));
    } else if (WIFSIGNALED(status)) {
        Nan::Set(result, String::NewFromUtf8(isolate, "exitCode", v8::NewStringType::kNormal).ToLocalChecked(), Null(isolate));
        Nan::Set(result, String::NewFromUtf8(isolate, "signalCode", v8::NewStringType::kNormal).ToLocalChecked(), Number::New(isolate, WTERMSIG(status)));
    }

    args.GetReturnValue().Set(result);
}

void Initialize(Local<Object> exports) {
    Isolate* isolate = exports->GetIsolate();

    NODE_SET_METHOD(exports, "waitpid", Waitpid);
    // expose the option constants
    Nan::Set(exports, String::NewFromUtf8(isolate, "WNOHANG", v8::NewStringType::kNormal).ToLocalChecked(), Number::New(isolate, WNOHANG));
    Nan::Set(exports, String::NewFromUtf8(isolate, "WUNTRACED", v8::NewStringType::kNormal).ToLocalChecked(), Number::New(isolate, WUNTRACED));
    Nan::Set(exports, String::NewFromUtf8(isolate, "WCONTINUED", v8::NewStringType::kNormal).ToLocalChecked(), Number::New(isolate, WCONTINUED));
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)
