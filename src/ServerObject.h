#ifndef ServerObject_h
#define ServerObject_h

#include <Arduino.h>
#include <WiFi.h>
#include <vector>
#include "ChainArray.h"
#include "Utils.h"
#include "Html.h"

#define RESPTYPE_HTML "text/html"
#define RESPTYPE_JSON "application/json"
#define RESPTYPE_CSV "text/csv"

#define METHOD_GET "GET"
#define METHOD_POST "POST"

class Html;
class Utils;
class ServerObject{
  public:
    void addServer(uint16_t port);
    void addServer(std::vector<uint16_t> ports);
    void openServer(uint16_t port);
    void openServer(std::vector<uint16_t> ports);
    void openAllServers();
    void requestHandle(uint16_t port);
    void requestHandle(std::vector<uint16_t> ports);
    void setResponse(uint16_t port, String url, Html *response, String method, String respType = RESPTYPE_HTML);
    void setNotFound(String resp);
    bool removeResponse(uint16_t port, String path, String method);

  private:
    void addServer_proc(uint16_t port);
    void openServer_proc(uint16_t port);
    void requestHandle_proc(uint16_t port);
    void requestHandle_get(WiFiClient client, uint16_t serverIndex, ChainArray request);
    void requestHandle_post(WiFiClient client, uint16_t serverIndex, ChainArray request);
    void sendGetResponseHeader(WiFiClient *client, String status, String respType);
    void sendGetResponseBody(WiFiClient *client, String html);
    struct Response{
      String url;
      String response;
      String respType;
      void (*prevCallback)(ChainArray, ChainArray, String*, WiFiClient*);
    };
    struct Server{
      uint16_t port;
      WiFiServer server;
      std::vector<struct Response> Responses;
      void setResponse(String url, Html *response, String respType){
        struct Response resObj;
        Html *test = response;

        resObj.respType = respType;
        resObj.url = url;
        resObj.response = response->getHtml();
        resObj.prevCallback = response->htmlObj.prev;
        Responses.push_back(resObj);
      };
      int16_t findPath(String path){
        int16_t result = -1;

        for(int i = 0; i < Responses.size(); i++){
          if(Responses[i].url == path) result = i;
        }
        return result;
      };
      void updateResponse(String url, Html *response, String respType){
        int16_t objIndex = findPath(url);

        if(objIndex >= 0){
          Responses[objIndex].respType = respType;
          Responses[objIndex].url = url;
          Responses[objIndex].response = response->getHtml();
          Responses[objIndex].prevCallback = response->htmlObj.prev;
        }
      };
      bool removeResponse(String path){
        for(int i = 0; i < Responses.size(); i++){
          if(Responses[i].url == path){
            Responses.erase(Responses.begin() + i);
            Responses.shrink_to_fit();
            return true;
          }
        }
        return false;
      }
    };
    String notFoundResp = "404";
    std::vector<struct Server> Servers_get;
    std::vector<struct Server> Servers_post;
    Utils *utils = new Utils();
};

#endif
