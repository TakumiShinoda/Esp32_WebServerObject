#ifndef WIFI_CONNECTION_H
#define WIFI_CONNECTION_H

#include <WiFi.h>

bool connectAP(const char *ssid, const char *pass, unsigned long connectWaitMillis = 10000, unsigned long disconnectWaitMillis = 1000, bool withSerialPrint = true);
void startAP(const char *ssid, const char *pass);
bool checkNetwork(String target);

#endif