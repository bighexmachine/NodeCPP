#include <node.h>
#include <stdio.h>
#include <wiringPi.h>
#include <iostream>
#include "myobject.h"

Persistent<Function> MyObject::constructor;

MyObject::MyObject(): state(0), delay(10), signals{1, 0, 16, 0}, clockIsRunning(0)
{
  clockLock.lock();
  clockThread = thread(&MyObject::Clock, this);
}

MyObject::~MyObject() {}

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
  NODE_SET_PROTOTYPE_METHOD(tpl, "writeData", WriteData);
  NODE_SET_PROTOTYPE_METHOD(tpl, "startClock", StartClock);
  NODE_SET_PROTOTYPE_METHOD(tpl, "stopClock", StartClock);
  NODE_SET_PROTOTYPE_METHOD(tpl, "stepClock", StartClock);
  NODE_SET_PROTOTYPE_METHOD(tpl, "ramPiSel", RamPiSel);
  NODE_SET_PROTOTYPE_METHOD(tpl, "reset", Reset);
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
  while(1)
  {
    clockLock.lock();
    writeClock( signals[state] );
    delayMicroseconds(delay);
    state = (state+1) % 4;
    clockLock.unlock();
  }
}

Handle<Value> MyObject::StartClock(const Arguments& args)
{
  HandleScope scope;
  MyObject* obj = ObjectWrap::Unwrap<MyObject>( args.This() );
  obj->clockIsRunning = 1;
  obj->clockLock.unlock();
  return scope.Close(Undefined());
}

Handle<Value> MyObject::StopClock(const Arguments& args)
{
  HandleScope scope;
  MyObject* obj = ObjectWrap::Unwrap<MyObject>( args.This() );
  obj->clockLock.lock();
  obj->clockIsRunning = 0;
  return scope.Close(Undefined());
}

Handle<Value> MyObject::StepClock(const Arguments& args)
{
  HandleScope scope;
  MyObject* obj = ObjectWrap::Unwrap<MyObject>( args.This() );
  if (obj->clockIsRunning) StopClock(args);
  writeClock( obj->signals[obj->state] );
  obj->state = (obj->state+1) % 4;
  return scope.Close(Undefined());
}

Handle<Value> MyObject::WriteData(const Arguments& args)
{
  HandleScope scope;
  int byte = args[0]->NumberValue();
  int base = 5;
  int i;
  for ( i=base; i<base+8; i++ )
  {
    int shift = i-base;
    digitalWrite (i, ( byte & (1<<shift) ) >> shift);
  }
  return scope.Close(Undefined());
}

Handle<Value> MyObject::RamPiSel(const Arguments& args)
{
  HandleScope scope;
  int input = args[0]->NumberValue();
  int bit = input & 1;
  digitalWrite (4, bit);
  return scope.Close(Undefined());
}

Handle<Value> MyObject::Reset(const Arguments& args)
{
  HandleScope scope;
  digitalWrite (1, 1);
  digitalWrite (3, 1);
  return scope.Close(Undefined());
}

