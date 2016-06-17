/*
   Copyright (c) 2015, Majenko Technologies
   All rights reserved.

   Redistribution and use in source and binary forms, with or without modification,
   are permitted provided that the following conditions are met:

 * * Redistributions of source code must retain the above copyright notice, this
     list of conditions and the following disclaimer.

 * * Redistributions in binary form must reproduce the above copyright notice, this
     list of conditions and the following disclaimer in the documentation and/or
     other materials provided with the distribution.

 * * Neither the name of Majenko Technologies nor the names of its
     contributors may be used to endorse or promote products derived fro
     this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
   ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
   ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
   ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/* Create a WiFi access point and provide a web server on it. */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <EEPROM.h>
#include <Arduino.h>
#include <Time.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsClient.h>
#include <Hash.h>
ESP8266WiFiMulti WiFiMulti;
WebSocketsClient webSocket;

/** the current address in the EEPROM (i.e. which byte we're going to write to next) **/
int addr = 0;

/* Set these to your desired credentials. */
const char *ap_ssid = "Window Sensor ";
const char *io_relay = "68.12.157.176";
int io_port = 4000;
//const char *ap_password = "password";
char html[5000] = "";
bool scan_complete = false;
bool ap_started = false;
bool connect_to_ap = false;
bool got_token = false;
const int led = 13;
int address = 0;
int ssid_ptr = 0;
int password_addr = 0;
byte value;
char response[130] = "";
char token_req[200] = "";
char is_token[130] = "";
char response_token[130] = "";
char ssid_found[50] = "";
char ssid[50] = "";
char new_password[50] = "";
char password[50] = "";
const int bufferSize = 2000;
uint8_t analog_data[bufferSize];
char analog_data_str[bufferSize] = "";
//uint8_t analog_buffer[44100];
byte mac[6];
char token[130] = "init";
char current_ssid[100];
bool isConnected = false;
char magnitude_str[50] = "";
int magnitude = 0;
char mac_addr[20] = "";
ESP8266WebServer server(80);

int ap_connect();
void start_ap();
/* Just a little test message.  Go to http://192.168.4.1 in a web browser
   connected to this access point to see it.
*/
void handleRoot() {
  server.send(200, "text/html", html);
}

void handleSensor() {
  digitalWrite ( led, 1 );
  char temp[400];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;
  snprintf ( temp, 400,
             "<html>\
  <head>\
    <meta http-equiv='refresh' content='5'/>\
    <title>ESP8266 Demo</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>Hello from ESP8266!</h1>\
    <p>Uptime: %02d:%02d:%02d</p>\
    <img src=\"/test.svg\" />\
  </body>\
</html>",
             hr, min % 60, sec % 60
           );
  server.send ( 200, "text/html", temp );
  digitalWrite ( led, 0 );
}

void handleNotFound() {
  digitalWrite ( led, 1 );
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for ( uint8_t i = 0; i < server.args(); i++ ) {
    message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
  }
  server.send ( 404, "text/plain", message );
  digitalWrite ( led, 0 );
}

void drawGraph() {
  String out = "";
  char temp[100];
  out += "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"400\" height=\"150\">\n";
  out += "<rect width=\"400\" height=\"150\" fill=\"rgb(250, 230, 210)\" stroke-width=\"1\" stroke=\"rgb(0, 0, 0)\" />\n";
  out += "<g stroke=\"black\">\n";
  int y = rand() % 130;
  for (int x = 10; x < 390; x += 10) {
    int y2 = rand() % 130;
    sprintf(temp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke-width=\"1\" />\n", x, 140 - y, x + 10, 140 - y2);
    out += temp;
    y = y2;
  }
  out += "</g>\n</svg>\n";
  server.send ( 200, "image/svg+xml", out);
}

int sample_rate = 20;
void get_analog_data() {
  for (int i = 0; i < bufferSize; i++) {
    analog_data[i] = analogRead(A0);
    magnitude += analog_data[i];
    if (analog_data[i]) {
      analog_data_str[i] = analog_data[i];
    } else {
      analog_data_str[i] = 'A';
    }
    delay( 1 / sample_rate );
  }
}

void store_wifi() {
  int j = 0;
  int i = 0;
  ssid_ptr = 0;

  strcpy(ssid, server.arg(0).c_str());
  strcpy(password, server.arg(1).c_str());

  char html2[500] = "";
  Serial.print("stored | SSID (");
  Serial.print(ssid);
  Serial.print(")  PASSWORD (");
  Serial.print(password);
  Serial.print(")\n");
  strcat(html2, "connecting to ");
  strcat(html2, ssid);
  strcat(html2, " with ");
  strcat(html2, password);
  strcat(html2, "<br><br>Enter your device ID (<b>");
  strcat(html2,mac_addr);  
  strcat(html2, "</b>) at <a href='http://dev.pyfi.org'>pyfi.org</a>");
  server.send(200, "text/html", html2);
  
  Serial.println(ssid);
  Serial.println(password);
  for (int i = 0; i < 100; i++) //clear some memory
    EEPROM.write(i, 0);

  for (i = 0; i < sizeof(ssid) ; i++) {
    if (ssid[i]) {
      Serial.print(" (loop) ");
      Serial.print(i);
      Serial.print(" ");
      Serial.println(ssid[i]);
      EEPROM.write(i, ssid[i]);
    } else {
      j = i;
      i = sizeof(ssid);
    }
  }
  Serial.print(" (adding) ");
  Serial.print(j);
  Serial.println(" null ");
  EEPROM.write(j, 0);
  j++;
  for (i = 0; i < sizeof(password) ; i++, j++) {
    if (password[i]) {
      Serial.print(" (loop) ");
      Serial.print(j);
      Serial.print(" ");
      Serial.println(password[i]);
      EEPROM.write(j, password[i]);
    } else {
      i = sizeof(password);
    }
  }
  Serial.print(" (adding) ");
  Serial.print(j);
  Serial.println(" null ");
  EEPROM.write(j, 0);
  EEPROM.commit();
  Serial.println((char)EEPROM.read(j + 1));

  if (ap_connect()) {
    Serial.println("connected to wifi");
    isConnected = true;
  } else {
    Serial.println("could not connect, starting access point...");
    start_ap();
    isConnected = false;
  }
}

void start_ap() {
  WiFi.mode(WIFI_AP);
  WiFi.disconnect();
  // WiFi.softAP(ap_ssid, ap_password);
  WiFi.softAP(current_ssid);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("gui access on http://");
  Serial.println(myIP);
}
void routes() {
  server.on("/", handleRoot);
  server.on("/store_wifi", store_wifi);
  server.on( "/sensor", handleSensor );
  server.on( "/test.svg", drawGraph );
  server.on( "/inline", []() {
    server.send( 200, "text/plain", "this works as well" );
  } );
  server.onNotFound ( handleNotFound );
  server.begin();
}

void scan() {
  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  scan_complete = true;
  strcat(html, "<form action='/store_wifi' method='get'>");
  strcat(html, "<select name='ssid'>");
  if (n == 0)
    Serial.println("no networks found");
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      //char ssid[50] = "";
      strcpy(ssid_found, WiFi.SSID(i).c_str());
      strcat(html, "<option value='");
      strcat(html, ssid_found);
      strcat(html, "'>");
      strcat(html, ssid_found);
      strcat(html, "</option>");
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
      delay(10);
    }
  }
  strcat(html, "</select><br>");
  strcat(html, "<input name='password' type='password' placeholder='password'></input><br>");
  strcat(html, "<input type='submit' value='Submit'></button></form>");
  //Serial.println(html);
  //start_ap();
}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t lenght) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.println("[WSc] Disconnected!\n");
      break;
    case WStype_CONNECTED:
      {
        Serial.printf("[WSc] Connected to url: %s\n",  payload);
        // send message to server when Connected
        char data[200] = "hello from ";
        strcat(data, current_ssid);
        webSocket.sendTXT(data);
        break;
      }
    case WStype_TEXT:
      Serial.print("[");
      Serial.print(mac_addr);
      Serial.print("]");
      Serial.printf("[WSc] get text: %s\n", payload);
      strcpy(response, (char *)payload);
      strncpy(is_token, response, 5);
      if (strcmp(is_token, "token") == 0) {
        for (int i = 0; i < sizeof(response); i++) {
          response_token[i] = response[i + 5];
        }
        Serial.print("setting token | ");
        Serial.println(response_token);
        strcpy(token, response_token);
        got_token = true;
      }
      // webSocket.sendTXT("message here");
      break;
    case WStype_BIN:
      Serial.printf("[WSc] get binary lenght: %u\n", lenght);
      hexdump(payload, lenght);
      // webSocket.sendBIN(payload, lenght);
      break;
  }
}

int ap_connect() {
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  Serial.print("connecting to ");
  Serial.print(ssid);
  Serial.print(" with ");
  Serial.println(password);
  WiFi.begin(ssid, password);
  int count = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    count++;
    if ( count == 20 ) {
      Serial.println();
      //start_ap();
      return 0;
    }
  }
  webSocket.onEvent(webSocketEvent);
  webSocket.begin(io_relay, 4000);
  //webSocket.begin("wss://pyfi-relay.herokuapp.com", 5000);
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());
  return 1;
}

void get_device_info() {
  Serial.println();
  Serial.println("getting wifi info...");
  //reading ssid
  for (int i = 0; i < sizeof(ssid); i++) {
    ssid[i] = (char)EEPROM.read(i);
    if (!ssid[i]) {
      password_addr = i + 1; //advance passed the null
      i = sizeof(ssid);
    }
  }
  //reading ssid password
  for (int i = 0; i < sizeof(password); i++, password_addr++) {
    password[i] = (char)EEPROM.read(password_addr);
    if (!password[i]) {
      i = sizeof(password);
    }
  }
  address = address + 1;
  // there are only 512 bytes of EEPROM, from 0 to 511, so if we're
  // on address 512, wrap around to address 0
  if (address == 100)
    address = 0;
}

void getmac() {
  WiFi.macAddress(mac);
  sprintf(mac_addr, "%02x%02x%02x%02x%02x%02x", mac[5], mac[4], mac[3], mac[2], mac[1], mac[0]);
  strcpy(current_ssid, ap_ssid);
  strcat(current_ssid, mac_addr);
}

void get_token() {
  strcpy(token_req, "{ \"mac\":\"");
  strcat(token_req, mac_addr);
  strcat(token_req, "\", \"device_type\":");
  strcat(token_req, "\"window_sensor\",");
  strcat(token_req, " \"token\":\"");
  strcat(token_req, token);
  strcat(token_req, "\", \"cmd\":");
  strcat(token_req, "\"tok_req\" }");
  Serial.println(token_req);
  if (isConnected) {
    webSocket.sendTXT(token_req);
  }
}

char local_ip[20] = "init";
void setup() {
  Serial.begin(115200);
  EEPROM.begin(512);
  getmac();
  scan();
  get_device_info();
  if (ap_connect()) {
    Serial.println("connected to wifi");
    isConnected = true;
  } else {
    Serial.println("could not connect, starting access point...");
    start_ap();
  }
  routes();
  String(WiFi.localIP().toString()).toCharArray(local_ip, 20);
}

int count = 0;
char count_str[50] = "";
char message[2100] = "";
char now_str[10] = "";
void loop() {
  server.handleClient();
  if (isConnected) {
    webSocket.loop(); 
  }
  get_analog_data();
  if (got_token) {
    if ( magnitude > 500 ) {
      String(now()).toCharArray(now_str, 10);
      String(magnitude).toCharArray(magnitude_str, 50);
      strcpy(message, "{ \"mac\":\"");
      strcat(message, mac_addr);
      strcat(message, "\", \"device_type\":");
      strcat(message, "\"window_sensor\", ");
      strcat(message,"\"local_ip\":\"");
      strcat(message, local_ip);
      strcat(message, "\", \"uptime\":");
      strcat(message, now_str);
      strcat(message, ", \"magnitude\":");
      strcat(message, magnitude_str);
      strcat(message, ", \"token\":\"");
      strcat(message, token);
      strcat(message, "\", \"data\":\"");
      //strcat(message,analog_data_str);
      strcat(message, "\" }");
      Serial.println(message);
      webSocket.sendTXT(message);
      if (strcmp(response, "ok") == 0) {
        strcpy(response, "reset");
      } else {
        Serial.print("no response from ");
        Serial.print(io_relay);
        Serial.print(":");
        Serial.println(io_port);
        count++;
        if (count > 3) {
          Serial.println("reconnecting...");
          webSocket.disconnect();
          delay(500);
          webSocket.begin(io_relay, 4000);
          count = 0;
        }        
      }      
    }
  } else {
    //delay(1000);
    get_token();
  }

  magnitude = 0;
  delay(100);
}
