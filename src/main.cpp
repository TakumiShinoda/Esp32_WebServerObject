#include <Arduino.h>

#include "ServerObject.h"
#include "local_property.h"
#include "WiFiConnection.h"
#include "Utils.h"

constexpr uint16_t PORT = 80;
constexpr unsigned long UART_BRT = 115200;

ServerObject server;
Utils utils;

void hogeCallback(ChainArray queries, ChainArray requests, String *response, WiFiClient *client){
  requests.showKeys(true);
  if(requests.exist("params")) Serial.println("found params");
  if(requests.exist("test")) Serial.println("found test");
}

void setup(){
  Html hogeHtml("hoge", &hogeCallback);
  Html postHtml("post", &hogeCallback);

  Serial.begin(UART_BRT);
  connectAP(SSID, PASS);

  server.addServer(PORT);
  server.setResponse(PORT, "/hoge", &hogeHtml, METHOD_GET);
  server.setResponse(PORT, "/hoge", &postHtml, METHOD_POST);

  server.openAllServers();
}

void loop(){
  server.requestHandle(80);
}