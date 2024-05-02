#include "./connectWifi.h"

void setup_connectWifi(){
  Serial.begin(115200);

  connectAP((const char*)SSID_STA, (const char*)PASS_STA, 200, 1000, false);
  if(connectAP("junichi_saikyo24", "godgodx4")) Serial.println("connected");
  else Serial.println("not connected");
}

void loop_connectWifi(){
  if(WiFi.status() != WL_CONNECTED){
    if(connectAP("junichi_saikyo24", "godgodx4")) Serial.println("connected");
    else Serial.println("not connected");
  }
}