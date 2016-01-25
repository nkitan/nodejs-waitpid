#include <v8.h>
#include <node.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>

using namespace v8;
using namespace node;

static Handle<Value> Waitpid(const Arguments& args) {
  HandleScope scope;
  int r, child, options, status;

  if (args[0]->IsInt32()) {
    child = args[0]->Int32Value();
  } else {
    return ThrowException(Exception::Error(String::New("Child PID must be an integer.")));
  }

  if (args[1]->IsInt32()) {
    options = args[1]->Int32Value();
  } else {
    return ThrowException(Exception::Error(String::New("Options must be an integer.")));
  }

  // call as nature intended
  // if you need to loop on this, then use WNOHANG and a timeout in node so you don't block
  r = waitpid(child, &status, options);

  Local<Object> result = Object::New();

  result->Set(String::New("pid"), Integer::New(r));

  if (WIFEXITED(status)) {
    result->Set(String::New("exitCode"), Integer::New(WEXITSTATUS(status)));
    result->Set(String::New("signalCode"), Null());
    return scope.Close(result);
  }
  else if (WIFSIGNALED(status)) {
    result->Set(String::New("exitCode"), Null());
    result->Set(String::New("signalCode"), Integer::New(WTERMSIG(status)));
    return scope.Close(result);
  }
  return scope.Close(Undefined());

}


extern "C" void init(Handle<Object> target) {
  HandleScope scope;
  NODE_SET_METHOD(target, "waitpid", Waitpid);
  // expose the option constants
  target->Set(String::NewSymbol("WNOHANG"), Integer::New(WNOHANG));
  target->Set(String::NewSymbol("WUNTRACED"), Integer::New(WUNTRACED));
  target->Set(String::NewSymbol("WCONTINUED"), Integer::New(WCONTINUED));
}


NODE_MODULE(waitpid, init)
