#include <node.h>
#include <stdio.h>
#include <wiringPi.h>
#include <iostream>
#define	DELAY	20000

using namespace v8;
using namespace std;

Handle<Value> Write(const Arguments& args) {
  HandleScope scope;

  if (args.Length() < 2) {
    ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
    return scope.Close(Undefined());
  }

  if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
    ThrowException(Exception::TypeError(String::New("Wrong argument types")));
    return scope.Close(Undefined());
  }

  double port = args[0]->NumberValue();
  double value = args[1]->NumberValue();

  if(port>12 || port<0){
    ThrowException(Exception::TypeError(String::New("Invlaid port Value")));
    return scope.Close(Undefined());
  }
  
  if (value == 1){
    digitalWrite (0, HIGH);
  }
  else if(value == 0) {
    digitalWrite (0, LOW);
  }
  else{
    ThrowException(Exception::TypeError(String::New("Invlaid write Value")));
  }
  return scope.Close(Undefined());
}

Handle<Value> CreateObject(const Arguments& args)
{
  HandleScope scope;

  Local<Object> obj = Object::New();
  obj->Set(String::NewSymbol("msg"), args[0]->ToString());

  return scope.Close(obj);
}

void Init(Handle<Object> exports, Handle<Object> module) {
  wiringPiSetup () ;

  int i;
  for(i=0;i<13;i++)
  {
    pinMode (i, OUTPUT) ;
  }

  exports->Set(String::NewSymbol("write"), FunctionTemplate::New(Write)->GetFunction());
  module->Set(String::NewSymbol("exports"), FunctionTemplate::New(CreateObject)->GetFunction());
}


NODE_MODULE(gpioService, Init)
