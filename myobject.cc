#include <node.h>
#include <stdio.h>
#include <wiringPi.h>
#include "myobject.h"

using namespace v8;

Persistent<Function> MyObject::constructor;

MyObject::MyObject() {};
MyObject::~MyObject() {};

void MyObject::Init(Handle<Object> target) {
  //setting up gpio pins
  wiringPiSetup () ;

  int i;
  for(i=0;i<13;i++)
  {
    pinMode (i, OUTPUT) ;
  }

  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  tpl->SetClassName(String::NewSymbol("MyObject"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  // Prototype
  NODE_SET_PROTOTYPE_METHOD(tpl, "write", Write);
  NODE_SET_PROTOTYPE_METHOD(tpl, "value", GetValue);
  NODE_SET_PROTOTYPE_METHOD(tpl, "plusOne", PlusOne);
  NODE_SET_PROTOTYPE_METHOD(tpl, "multiply", Multiply);

  constructor = Persistent<Function>::New(tpl->GetFunction());
  target->Set(String::NewSymbol("MyObject"), constructor);
}

Handle<Value> MyObject::New(const Arguments& args) {
  HandleScope scope;

  MyObject* obj = new MyObject();
  obj->value_ = args[0]->IsUndefined() ? 0 : args[0]->NumberValue();
  obj->Wrap(args.This());

  return args.This();
}

Handle<Value> MyObject::Write(const Arguments& args) {
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

Handle<Value> MyObject::GetValue(const Arguments& args) {
  HandleScope scope;

  MyObject* obj = ObjectWrap::Unwrap<MyObject>(args.Holder());

  return scope.Close(Number::New(obj->value_));
}

Handle<Value> MyObject::PlusOne(const Arguments& args) {
  HandleScope scope;

  MyObject* obj = ObjectWrap::Unwrap<MyObject>(args.This());
  obj->value_ += 1;

  return scope.Close(Number::New(obj->value_));
}

Handle<Value> MyObject::Multiply(const Arguments& args) {
  HandleScope scope;

  MyObject* obj = ObjectWrap::Unwrap<MyObject>(args.This());
  double multiple = args[0]->IsUndefined() ? 1 : args[0]->NumberValue();

  const int argc = 1;
  Local<Value> argv[argc] = { Number::New(obj->value_ * multiple) };

  return scope.Close(constructor->NewInstance(argc, argv));
}
