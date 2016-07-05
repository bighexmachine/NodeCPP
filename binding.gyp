{
  "targets": [
    {
      "target_name": "gpioService",
      "sources": [ "gpioService.cc", "myobject.cc" ],
      "libraries": ["-lwiringPi -lpthread"],
      "cflags": ["-std=c++0x"]
    }
  ]
}
