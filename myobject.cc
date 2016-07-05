#include <node.h>
#include <stdio.h>
#include <wiringPi.h>
#include <iostream>
#include "myobject.h"

using namespace v8;
using namespace std;

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
  NODE_SET_PROTOTYPE_METHOD(tpl, "startClock", StartClock);
  constructor = Persistent<Function>::New(tpl->GetFunction());
  target->Set(String::NewSymbol("MyObject"), constructor);
}

Handle<Value> MyObject::New(const Arguments& args) {
  HandleScope scope;

  MyObject* obj = new MyObject();
  obj->value_ = args[0]->IsUndefined() ? 0 : args[0]->NumberValue();
  obj->Wrap(args.This());
  obj->bob =  10;
  return args.This();
}

void writeClock(int val)
{
  //phase 0 clock
  digitalWrite (0, val & 1);
  //phase 0 reset
  digitalWrite (1, val & 2);
  //phase 1 clock
  digitalWrite (2, val & 16);
  //phase 1 reset
  digitalWrite (3, val & 32);
}

void MyObject::Clock()
{
  cout << "started";
  int[] signals = {1, 0, 16, 0}
  while(1)
  {
    writeClock( signals[state] );
    delayMicroseconds(100000);
    state = (state+1) % 4;
  }
}

Handle<Value> MyObject::StartClock(const Arguments& args)
{
  HandleScope scope;
  MyObject* obj = ObjectWrap::Unwrap<MyObject>( args.This() );
  obj->clockThread = thread(&MyObject::Clock, obj);
  //cout <<  obj->value_ << " " << obj->bob << endl;
  return scope.Close(Undefined());
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

