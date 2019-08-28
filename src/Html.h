#ifndef Html_h
#define Html_h

#include <Arduino.h>
#include <vector>
#include <WiFi.h>
#include "ChainArray.h"

class Html{
  private:
    struct HtmlObject{
      String html;
      void (*prev)(ChainArray, ChainArray, String*, WiFiClient*);
    };

  public:
    Html(String html, void (*prev)(ChainArray, ChainArray, String*, WiFiClient*));
    String getHtml();
    struct HtmlObject htmlObj;
};

#endif
