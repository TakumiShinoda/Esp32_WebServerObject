#include <WiFiMulti.h>

#define TRY_CONNECT_AP 50

bool connectAP(const char *ssid, const char *pass){
  uint8_t cnt = 0;

  WiFi.mode(WIFI_MODE_APSTA);
  WiFi.disconnect(true);
  delay(1000);
  WiFiMulti wifi;

  wifi.addAP(ssid, pass);
  while (wifi.run() != WL_CONNECTED && cnt < TRY_CONNECT_AP){
    delay(500);
    Serial.print(".");
    cnt += 1;
  }
  if(wifi.run() != WL_CONNECTED) return false;

  Serial.println("\nWiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  return true;
}

void startAP(const char *ssid, const char *pass){
  WiFi.mode(WIFI_MODE_APSTA);
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
