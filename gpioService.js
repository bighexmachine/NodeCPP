var service = require('bindings')('gpioService');

var obj = new service.MyObject(10);

var stdin = process.openStdin()
stdin.addListener("data", function(d){

  if ( d.toString().trim() == "start" ){
      obj.startClock();
  } else if ( d.toString().trim() == "stop" ){
      obj.stopClock();
  } else if ( d.toString().trim() == "step" ){
      obj.stepClock();
  } else if ( d.toString().trim() == "reset" ){
      obj.reset();
  }
  //obj.write(0, parseInt(d.toString().trim()) )
})
