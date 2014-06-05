var net = require('net');
var express = require('express');
var http = require('http');

var HOST = '0.0.0.0';
var MSP_PORT = 1234;
var HTML_PORT = 3000;
var socket;

var app = express();
var server = net.createServer(function(c) {
  socket = c;
  c.on('end', function() {
    socket = null;
  });
});

var state = { button: {orientation: "up", presses: 0},
              handshake: "no" };

app.use("/", express.static(__dirname + '/public'));

app.post("/state", function(request, response) {
  response.send(JSON.stringify(state));
});

app.post("/blink", function(request, response) {
  console.log("Blink");
  socket.write("blink");
  response.send("blinked");
});

server.on("connection", function(sock) {
  console.log('CONNECTED: ' + sock.remoteAddress +':'+ sock.remotePort);
  
  sock.on("data", function(data) {
    console.log('DATA: ' + data);
    if (data == "Button down\n") {
      state.button.presses++;
      state.button.orientation = "down";
    } else if (data == "Button up\n") {
      state.button.orientation = "up";
    } else if (data == "Handshake complete!\n") {
      state.handshake = "yes";
    }
  });
});

app.listen(HTML_PORT);
server.listen(MSP_PORT, HOST);
