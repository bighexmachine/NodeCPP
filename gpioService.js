var gpioService = require('bindings')('gpioService.node')

var a = gpioService("name1")
console.log(a.msg)

var stdin = process.openStdin()
stdin.addListener("data", function(d){
  gpioService.write(0, parseInt(d.toString().trim()) )
})

