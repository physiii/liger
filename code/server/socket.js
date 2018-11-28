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
const server = http.createServer().listen(5050);


server.on('upgrade', (request, socket, head) => {
  const pathname = url.parse(request.url).pathname;
  console.log("http upgrade request: ", request.headers);
  if (pathname === '/device-relay') {
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

//setInterval(function(){ console.log(Date.now()) }, 3000);
wssMain.on('connection', function connection(ws, req) {

  console.log("<< ---- incoming connection, sending token ---- >>");
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
        console.log(msg.payload);
        //ws.send("{\"event_type\":\"token\", \"payload\":{\"token\":\"25dc4876-d1e2-4d6e-ba4f-fba81992c888\"}}");

        let response = {event_type:"schedule"};
        let payload = {action:"remove"};
        payload.seconds_into_day = 44550;
        payload.state = {level:255,on:true};
        payload.service_id = "dimmer_1"
        payload.event_id = "b321"
        response.id = 321;
        response.payload = payload;
        response = JSON.stringify(response);
        //ws.send(response);
        console.log(response);
        break;

      case "load":
        console.log(msg.payload);
        ws.send("{\"event_type\":\"load\", \"payload\":{\"result\":\"services_loaded\"}}");
        break;

      case "time":
        console.log(msg.payload);
        ws.send("{\"event_type\":\"time\", \"payload\":{\"time\":"+Date.now()+"}}");
        break;

      case "service/state":
        console.log(msg.payload);
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
