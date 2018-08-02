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
const wssToken = new WebSocketServer({ noServer: true });
const wssButtons = new WebSocketServer({ noServer: true });
const wssPower = new WebSocketServer({ noServer: true });
const wssLED = new WebSocketServer({ noServer: true });
const wssMicrophone = new WebSocketServer({ noServer: true });
const wssMotion = new WebSocketServer({ noServer: true });
const wssUpdate = new WebSocketServer({ noServer: true });
const wssClimate = new WebSocketServer({ noServer: true });
const wssTest = new WebSocketServer({ noServer: true });
const server = http.createServer().listen(4000);


server.on('upgrade', (request, socket, head) => {
  const pathname = url.parse(request.url).pathname;

  if (pathname === '/token') {
    wssToken.handleUpgrade(request, socket, head, (ws) => {
      wssToken.emit('connection', ws);
    });
  } else if (pathname === '/buttons') {
    wssButtons.handleUpgrade(request, socket, head, (ws) => {
      wssButtons.emit('connection', ws);
    });
  } else {
    socket.destroy();
  }
});

wssTest.on('connection', function connection(ws) {
  console.log("connection established");
  ws.on('message', function incoming(message) {
    console.log("<< ---- incoming test message ---- >>\n", message);
  });
});

wssToken.on('connection', function connection(ws) {
  console.log("<< ---- incoming token connection ---- >>");
  ws.on('message', function incoming(message) {
    console.log("<< ---- incoming token message ---- >>\n", message);
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
      case "getToken":
        
        ws.send("TEST-TOKEN");
        break;
      default:
        console.log("unknown event type: ",event_type);
    }
    /*var msg = {};
    try { msg = JSON.parse(message) }
    catch (e) { console.log("invalid json", message) };
    var token = msg.token;
    var mac = msg.mac;
    var cmd = msg.cmd;
    var type = msg.type;
    var local_ip = msg.local_ip;
    var public_ip = ws.upgradeReq.connection.remoteAddress;
    var device_index = find_index(device_objects, 'token', token);
    //console.log(TAG, "device_objects ", device_objects);
    if (!type) return;

    // --------------  respond to ping requests  ----------------- //
    if (cmd == "png_test") {
      command = "png_test";
      try { ws.send('command' + command) }
      catch (e) { console.log("reply error | " + e) };
      ping_time = Date.now() - ping_start;
      //console.log(mac + " | received ping, sending reply ");
    }

    // ------------------  send device info  --------------------- //
    if (cmd == "version") {
      if (!device_objects[device_index])
        return;
      for (var j = 0; j < device_objects[device_index].groups.length; j++) {
        message_user(device_objects[device_index].groups[j], 'version', msg);
        var group_index = find_index(groups, 'group_id', device_objects[device_index].groups[j]);
        console.log("media_controller messing users", device_objects[device_index].groups[j]);
        for (var k = 0; k < groups[group_index].members.length; k++) {
          message_device(groups[group_index].members[k], msg);
          message_user(groups[group_index].members[k], 'version', msg);
        }
      }

      console.log("sending version number", msg);
    }

    // --------------  respond to token requests  ----------------- //
    if (cmd == "token_request") {
      var token = crypto.createHash('sha512').update(mac).digest('hex');
      console.log(TAG, (Date.now() - start_time) / 1000, "token_request", token);
      try { ws.send('{\"token\":\"' + token + '\"}') }
      //try { ws.send(token) }
      catch (e) { console.log("reply error | " + e) };
      var device_index = find_index(device_objects, 'mac', mac);
      if (device_index < 0) {
        var device_object = { token: token, mac: mac, local_ip: local_ip, public_ip: public_ip, type: [type], groups: [] };
        database.store_device_object(device_object);
        console.log('added device', device_object.mac);
        device_object.wsToken = ws;
        device_objects.push(device_object);
      } else {
        device_objects[device_index].token = token;
        device_objects[device_index].public_ip = public_ip;
        device_objects[device_index].local_ip = local_ip;
        device_objects[device_index].type = type;
        //if (device_objects[device_index].wsToken) delete device_objects[device_index].wsToken;
        //database.store_device_object(device_objects[device_index]);
        device_objects[device_index].wstoken = ws;
        //console.log('updated device',device_objects[device_index]);
      }

      var index = find_index(groups, 'group_id', mac);
      if (index < 0) {
        var group = { group_id: mac, mode: 'init', type: [type], members: [mac], IR: [], record_mode: false };
        groups.push(group);
        database.store_group(group);
      }
    }*/

    // -------- //
  });

  ws.on('close', function close() {
    for (var i = 0; i < device_objects.length; i++) {
      _socket = device_objects[i].socket;
      _mac = device_objects[i].mac;
      if (_socket === ws) {
        device_objects.slice(i); //slice or splice??
        console.log(_mac + " | disconnected");
      }
    }
  });

  ws.on('error', function () {
    console.log('device websocket error catch!');
  });
});

function message_device(token, msg) {
  var device_index = find_index(device_objects, 'token', token)
  if (device_index > -1)
    if (device_objects[device_index].socket)
      device_objects[device_index].socket.emit(msg.type, msg);
}
