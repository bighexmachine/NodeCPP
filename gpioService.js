var service = require('bindings')('gpioService');

var obj = new service.MyObject(10);

var stdin = process.openStdin()
stdin.addListener("data", function(d){
  obj.startClock();
  //obj.write(0, parseInt(d.toString().trim()) )
})
