var service = require('bindings')('gpioService');

var obj = new service.MyObject(10);

var stdin = process.openStdin()
stdin.addListener("data", function(d){

  if ( d.toString().trim() == "start" ){
      obj.startClock();
      console.log("started");
  } else if ( d.toString().trim() == "stop" ){
      obj.stopClock();
      console.log("stopped");
  } else if ( d.toString().trim() == "step" ){
      obj.stepClock();
      console.log("stepped");
  } else if ( d.toString().trim() == "reset" ){
      obj.reset();
      console.log("reset");
  } else {
    obj.setSpeed(parseInt( d.toString().trim() ))
  }
  //obj.write(0, parseInt(d.toString().trim()) )
})
