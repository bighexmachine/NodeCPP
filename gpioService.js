var gpioService = require('bindings')('gpioService.node')
var stdin = process.openStdin()
stdin.addListener("data", function(d){
  gpioService.write(0, parseInt(d.toString().trim()) )
})

