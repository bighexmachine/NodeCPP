#include <node.h>
#include "gpioController.h"

using namespace v8;

void InitAll(Handle<Object> exports) {
  MyObject::Init(exports);
}

NODE_MODULE(gpioService, InitAll)
