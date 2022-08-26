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
    int waitpid_result, status;

    // check arguments
    if (args.Length() < 2) {
        Nan::ThrowTypeError("Two arguments are required, PID and options");
        return;
    }
    if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
        Nan::ThrowTypeError("PID and options must be numbers");
        return;
    }

    // do the waitpid call
    waitpid_result = waitpid(args[0]->NumberValue(Nan::GetCurrentContext()).ToChecked(), &status, args[1]->NumberValue(Nan::GetCurrentContext()).ToChecked());

    // set return value to waitpid result
    Local<Object> result = Object::New(isolate);
    Nan::Set(result, String::NewFromUtf8(isolate, "return", v8::NewStringType::kNormal).ToLocalChecked(), Number::New(isolate, waitpid_result));

    if (WIFEXITED(status)) {
        // program exited
        Nan::Set(result, String::NewFromUtf8(isolate, "exitCode", v8::NewStringType::kNormal).ToLocalChecked(), Number::New(isolate, WEXITSTATUS(status)));
        Nan::Set(result, String::NewFromUtf8(isolate, "signalCode", v8::NewStringType::kNormal).ToLocalChecked(), Null(isolate));
    } else if (WIFSIGNALED(status)) {
        // program terminated
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
