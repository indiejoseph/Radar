fs = require "fs"
url = require "url"
http = require "http"

{SerialPort}  = require "serialport"
portName = "/dev/tty.usbmodemfa1341"

# static file http server
# serve files for application directory

root = "public"
val = ""
http = http.createServer (req, res) ->
	request = url.parse req.url, false

	console.log "Serving request: #{request.pathname}"

	filename = request.pathname
	filename = "/index.html" if filename is "/"
	filename = root + filename

	try
		fs.realpathSync(filename)
	catch e
		res.writeHead(404)
		res.end()

	contentType = "text/plain"

	if filename.match(".js$")
		contentType = "text/javascript"
	else if filename.match(".html$")
		contentType = "text/html"
	else if filename.match(".css$")
		contentType = "text/css"

	fs.readFile filename, (err, data) ->
		if err
			res.writeHead 500
			res.end()
			return

		res.writeHead 200, {"Content-Type": contentType}
		res.write data
		res.end()

http.listen 3000

console.log "server started on localhost:3000"

# Socket.io config

io = require("socket.io").listen http # server listens for socket.io communication at port 3000
io.set "log level", 1 # disables debugging. this is optional. you may remove it if desired.

sp = new SerialPort portName, {
	baudrate: 9600
} # instantiate the serial port.

sp.on "close", (err) -> console.log "port closed"
sp.on "error", (err) -> console.error "error", err
sp.on "open",  ->  console.log "Arduino connected."

io.sockets.on "connection", (socket) ->
    # If socket.io receives message from the client browser then 
    # this call back will be executed.

	console.log "Websocket opened"
	
	socket.on "message", (data) ->
		console.log data
	
	# If a web browser disconnects from Socket.IO then this callback is called.
	socket.on "disconnect", -> console.log "disconnected"

# call back when data is received
sp.on "data", (data) ->
	
	val +=  data.toString()
	
	if val.substr(val.length - 1, 1) == "}"
		io.sockets.emit "message", val
		val = ""