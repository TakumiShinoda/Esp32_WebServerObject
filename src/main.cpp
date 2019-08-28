#include <Arduino.h>

#include "ServerObject.h"
#include "local_property.h"
#include "WiFiConnection.h"

constexpr uint16_t PORT = 80;

ServerObject server;

void hogeCallback(ChainArray queries, ChainArray requests, String *response, WiFiClient *client){

}

void setup(){
  Html hogeHtml("hoge", &hogeCallback);

  connectAP(SSID, PASS);

  server.addServer(PORT);
  server.setResponse(PORT, "/hoge", &hogeHtml);

  server.openAllServers();
}

void loop(){
  server.requestHandle(80);
}