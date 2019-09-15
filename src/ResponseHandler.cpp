#include "ResponseHandler.h"

ResponseHandler::ResponseHandler(String html, void (*prev)(ChainArray, ChainArray, String*, WiFiClient*)){
  htmlObj.html = html;
  htmlObj.prev = prev;
}

String ResponseHandler::getResponseHandler(){
  return htmlObj.html;
}
