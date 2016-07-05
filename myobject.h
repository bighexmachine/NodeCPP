#ifndef MYOBJECT_H
#define MYOBJECT_H
#include <thread>
#include <node.h>

class MyObject : public node::ObjectWrap {
 public:
  static void Init(v8::Handle<v8::Object> target);

 private:
  MyObject();
  ~MyObject();

  static v8::Handle<v8::Value> New(const v8::Arguments& args);
  static v8::Handle<v8::Value> Write(const v8::Arguments& args);
  static v8::Handle<v8::Value> StartClock(const v8::Arguments& args);
  static v8::Persistent<v8::Function> constructor;
  double value_;
  std::thread clockThread;
  int bob;
};

#endif
