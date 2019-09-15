#ifndef ResponseHandler_h
#define ResponseHandler_h

#include <Arduino.h>
#include <vector>
#include <WiFi.h>
#include "ChainArray.h"

class ResponseHandler{
  private:
    struct ResponseHandlerObject{
      String html;
      void (*prev)(ChainArray, ChainArray, String*, WiFiClient*);
    };

  public:
    ResponseHandler(String html, void (*prev)(ChainArray, ChainArray, String*, WiFiClient*));
    String getResponseHandler();
    struct ResponseHandlerObject htmlObj;
};

#endif
