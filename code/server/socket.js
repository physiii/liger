// ------------------------------  OPEN-AUTOMATION ----------------------------------- //
// -----------------  https://github.com/physiii/open-automation  -------------------- //
// ---------------------------------- socket.js -------------------------------------- //

var database = require('./database.js');
var utils = require('./utils.js');
var crypto = require('crypto');
var http = require('http');
var https= require('https');
var fs = require('fs');
var url = require('url');

/* --------------  websocket server for devices  ----------------- */
var WebSocketServer = require('ws').Server
const wssMain = new WebSocketServer({ noServer: true });
const server = http.createServer().listen(5000);


server.on('upgrade', (request, socket, head) => {
  const pathname = url.parse(request.url).pathname;
  console.log("http upgrade request: ", request.headers);
  if (pathname === '/') {
    wssMain.handleUpgrade(request, socket, head, (ws) => {
      wssMain.emit('connection', ws);
    });
  } else if (pathname === '/buttons') {
    wssButtons.handleUpgrade(request, socket, head, (ws) => {
      wssButtons.emit('connection', ws);
    });
  } else {
    socket.destroy();
  }
});

wssMain.on('connection', function connection(ws, req) {

  console.log("<< ---- incoming connection, sending token ---- >>");
  ws.send("{\"token\":\"25dc4876-d1e2-4d6e-ba4f-fba81992c999\"}");
  ws.on('message', function incoming(message) {
    console.log("<< ---- incoming message ---- >>\n", message);
    var msg = ""
    try { msg = JSON.parse(message) }
    catch (e) { console.log("invalid json", message) };
    if (!msg.event_type) return;
    let event_type = msg.event_type;
    switch (event_type) {
      case "getUUID":
        console.log("sending UUID...");
        ws.send("{uuid:\"25dc4876-d1e2-4d6e-ba4f-fba81992c888\"}");
        break;

      case "button/pressed":
        //console.log("pressed:",msg.payload);
        console.log(msg.payload);
        //ws.send("{\"event_type\":\"switch\", \"payload\":{\"level\":100}}");
        break;


      default:
        console.log("unknown event type: ",event_type);
    }
  });

  ws.on('close', function close() {
  });

  ws.on('error', function () {
    console.log('device websocket error catch!');
  });
});
