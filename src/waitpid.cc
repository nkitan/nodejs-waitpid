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
        isolate->ThrowException(Exception::TypeError(
            String::NewFromUtf8(isolate, "Two arguments are required, PID and options").ToLocalChecked()));
        return;
    }
    if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
        isolate->ThrowException(Exception::TypeError(
            String::NewFromUtf8(isolate, "PID and options must be numbers").ToLocalChecked()));
        return;
    }

    // do the waitpid call
    r = waitpid(args[0]->NumberValue(Nan::GetCurrentContext()).ToChecked(), &status, args[1]->NumberValue(Nan::GetCurrentContext()).ToChecked());

    // return an object
    Local<Object> result = Object::New(isolate);

    result->Set(String::NewFromUtf8(isolate, "return").ToLocalChecked(), Number::New(isolate, r));

    if (WIFEXITED(status)) {
        result->Set(String::NewFromUtf8(isolate, "exitCode").ToLocalChecked(), Number::New(isolate, WEXITSTATUS(status)));
        result->Set(String::NewFromUtf8(isolate, "signalCode").ToLocalChecked(), Null(isolate));
    } else if (WIFSIGNALED(status)) {
        result->Set(String::NewFromUtf8(isolate, "exitCode").ToLocalChecked(), Null(isolate));
        result->Set(String::NewFromUtf8(isolate, "signalCode").ToLocalChecked(), Number::New(isolate, WTERMSIG(status)));
    }

    args.GetReturnValue().Set(result);
}

void Initialize(Local<Object> exports) {
    Isolate* isolate = exports->GetIsolate();

    NODE_SET_METHOD(exports, "waitpid", Waitpid);
    // expose the option constants
    exports->Set(String::NewFromUtf8(isolate, "WNOHANG").ToLocalChecked(), Number::New(isolate, WNOHANG));
    exports->Set(String::NewFromUtf8(isolate, "WUNTRACED").ToLocalChecked(), Number::New(isolate, WUNTRACED));
    exports->Set(String::NewFromUtf8(isolate, "WCONTINUED").ToLocalChecked(), Number::New(isolate, WCONTINUED));
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)
