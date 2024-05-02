#include "./WifiConnection.h"

bool connectAP(const char *ssid, const char *pass, unsigned long connectWaitMillis, unsigned long disconnectWaitMillis, bool withSerialPrint){
  unsigned long startTime;
  bool connectionResult = true;

  if(withSerialPrint) Serial.println("WiFi initializing");
  WiFi.disconnect(true, true);
  startTime = millis();
  while(WiFi.status() == WL_CONNECTED){
    if(withSerialPrint) Serial.print(".");

    if((millis() - startTime) > disconnectWaitMillis) return false;

    vTaskDelay(100);
  }

  if(withSerialPrint) Serial.println("WiFi connecting");
  WiFi.mode(WIFI_MODE_STA);
  WiFi.begin(ssid, pass);
  startTime = millis();
  while(WiFi.status() != WL_CONNECTED){
    if(withSerialPrint) Serial.print(".");
    
    if((millis() - startTime) > connectWaitMillis){
      connectionResult = false;
      break;
    }

    vTaskDelay(100);
  }
  if(withSerialPrint) Serial.println("");

  if(connectionResult && withSerialPrint){
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }else WiFi.disconnect(true, true);

  return connectionResult;
}

void startAP(const char *ssid, const char *pass){
  WiFi.mode(WIFI_MODE_AP);
  WiFi.softAP(ssid, pass);
}

bool checkNetwork(String target){
  bool result = false;
  uint16_t nets = WiFi.scanNetworks();

  for(int i = 0; i < nets; i++){
    String ssid = WiFi.SSID(i);

    if(ssid == target){
      result = true;
      break;
    }
  }

  return result;
}
