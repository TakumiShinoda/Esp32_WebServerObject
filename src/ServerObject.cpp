#include "ServerObject.h"

void ServerObject::addServer(uint16_t port){
  addServer_proc(port);
}

void ServerObject::addServer(std::vector<uint16_t> ports){
  for(int i = 0; i < ports.size(); i++){
    addServer_proc(ports[i]);
  }
}

void ServerObject::addServer_proc(uint16_t port){
  struct Server server;
  WiFiServer s(port);

  server.port = port;
  server.server = s;

  Servers_get.push_back(server); 
  Servers_post.push_back(server);
}

void ServerObject::openAllServers(){
  for(int i = 0; i < Servers_get.size(); i++){
    Servers_get[i].server.begin();
  }
}

void ServerObject::openServer(uint16_t port){
  openServer_proc(port);
}

void ServerObject::openServer(std::vector<uint16_t> ports){
  for(int i = 0; i < ports.size(); i++){
    openServer_proc(ports[i]);
  }
}

void ServerObject::openServer_proc(uint16_t port){
  for(int i = 0; i < Servers_get.size(); i++){
    if(Servers_get[i].port == port){
      Servers_get[i].server.begin();
      break;
    }
  }
}

void ServerObject::requestHandle(uint16_t port){
  requestHandle_proc(port);
}

void ServerObject::requestHandle(std::vector<uint16_t> ports){
  for(int i = 0; i < ports.size(); i++){
    requestHandle_proc(ports[i]);
  }
}

void ServerObject::requestHandle_proc(uint16_t port){
  uint16_t serverIndex = 0; 
  String line = "";
  String path = "";
  ChainArray request;
  ChainArray queries;
  WiFiClient client;
  std::vector<String> keys;

  for(int i = 0; i < Servers_get.size(); i++){
    if(Servers_get[i].port != port) continue;

    serverIndex = i;
    client = Servers_get[serverIndex].server.available();
    break;
  } 

  if(!client) return;
  if(!client.connected()) return;

  Serial.println("New Client");
  while(client.available()){
    line = client.readStringUntil('\n');
    path = "";
    request = utils->analyzeRequestLine(line); 
    request.add("port", String(Servers_get[serverIndex].port));

    if(line.indexOf("GET") == 0){
      request.add("method", "GET");

      requestHandle_get(client, serverIndex, request);
    }else if(line.indexOf("POST") == 0){
      request.add("method", "POST");

      requestHandle_post(client, serverIndex, request);
    }
  }
}

void ServerObject::requestHandle_get(WiFiClient client, uint16_t serverIndex, ChainArray request){ 
  uint8_t serverPort = request.get("port").toInt();
  String path = request.get("path");
  ChainArray queries = utils->analyzeQuery(request.get("params"));

  Serial.print("From: ");
  Serial.println(client.remoteIP());

  Serial.print("Path: ");
  Serial.println(path);

  Serial.print("port: ");
  Serial.println(serverPort);

  if(Servers_get[serverIndex].Responses.size() == 0){
    sendGetResponseHeader(&client, "404", RESPTYPE_HTML);
    sendGetResponseBody(&client, notFoundResp);
  }else{
    for(int i = 0; i < Servers_get[serverIndex].Responses.size(); i++){
      String registeredPath = Servers_get[serverIndex].Responses[i].url;

      if(path == registeredPath){
        String respHtml = Servers_get[serverIndex].Responses[i].response;

        sendGetResponseHeader(&client, queries.get("ResponseStatus"), Servers_get[serverIndex].Responses[i].respType);
        Servers_get[serverIndex].Responses[i].prevCallback(queries, request, &respHtml, &client);
        sendGetResponseBody(&client, respHtml);
        break;
      }else if(
        registeredPath.substring(registeredPath.length() - 2) == "/*"  && 
        path.indexOf(registeredPath.substring(0, registeredPath.length() - 2)) == 0 
      ){
        String respHtml = Servers_get[serverIndex].Responses[i].response;
        String wildRootPath = registeredPath.substring(0, registeredPath.length() - 2);

        request.add("wildRootPath", wildRootPath);
        request.add("wildPath", path.substring(wildRootPath.length()));
        sendGetResponseHeader(&client, queries.get("ResponseStatus"), Servers_get[serverIndex].Responses[i].respType);
        Servers_get[serverIndex].Responses[i].prevCallback(queries, request, &respHtml, &client);
        sendGetResponseBody(&client, respHtml);
        break;
      }
      if(i == Servers_get[serverIndex].Responses.size() - 1){
        sendGetResponseHeader(&client, "404", RESPTYPE_HTML);
        sendGetResponseBody(&client, notFoundResp);
        break;
      }
    }
  }
  client.stop();
}

void ServerObject::requestHandle_post(WiFiClient client, uint16_t serverIndex, ChainArray request){
  bool bodyFlag = false;
  String body = "";
  String contentType = ""; 
  ChainArray queries = ChainArray();
  String path = request.get("path");
  uint8_t serverPort = request.get("port").toInt();

  Serial.println("POST");

  while(client.available()){
    String postLine = client.readStringUntil('\n');

    postLine.replace("\r", "");
    if(postLine.indexOf("Content-Type:") == 0){
      contentType = postLine.substring(13);
      contentType.replace(" ", "");
      contentType.replace("\n", "");
      contentType.replace("\r", "");
    }
    if(bodyFlag) body += postLine;
    if(postLine == "") bodyFlag = true;
  }

  if(contentType == RESPTYPE_JSON){
    ChainArray forJsonContent = ChainArray();

    forJsonContent.add("body", body);
    queries = forJsonContent;
  }else queries = utils->analyzeQuery(body);
  request.add("contentType", contentType);

  utils->debugPrint("Content-Type", contentType);
  utils->debugPrint("Body", body);
  utils->debugPrint("From", String(client.remoteIP()));
  utils->debugPrint("Path", path);
  utils->debugPrint("Port", String(serverPort));

  if(Servers_post[serverIndex].Responses.size() == 0){
    sendGetResponseHeader(&client, "404", RESPTYPE_HTML);
    sendGetResponseBody(&client, notFoundResp);
    return;
  }

  for(int i = 0; i < Servers_post[serverIndex].Responses.size(); i++){
    String registeredPath = Servers_post[serverIndex].Responses[i].url;

    if(path == registeredPath){
      String respHtml = Servers_post[serverIndex].Responses[i].response;

      sendGetResponseHeader(&client, queries.get("ResponseStatus"), Servers_post[serverIndex].Responses[i].respType);
      Servers_post[serverIndex].Responses[i].prevCallback(queries, request, &respHtml, &client);
      sendGetResponseBody(&client, respHtml);
      break;
    }else if(
      registeredPath.substring(registeredPath.length() - 2) == "/*" && 
      path.indexOf(registeredPath.substring(0, registeredPath.length() - 1)) == 0 
    ){
      String respHtml = Servers_post[serverIndex].Responses[i].response;
      String wildRootPath = registeredPath.substring(0, registeredPath.length() - 1);

      request.add("wildRootPath", wildRootPath);
      request.add("wildPath", path.substring(wildRootPath.length()));
      sendGetResponseHeader(&client, queries.get("ResponseStatus"), Servers_post[serverIndex].Responses[i].respType);
      Servers_post[serverIndex].Responses[i].prevCallback(queries, request, &respHtml, &client);
      sendGetResponseBody(&client, respHtml);
      break;
    }else if(i == Servers_post[serverIndex].Responses.size() - 1){
      sendGetResponseHeader(&client, "404", RESPTYPE_HTML);
      sendGetResponseBody(&client, notFoundResp);
      break;
    }
  }
}

void ServerObject::sendGetResponseHeader(WiFiClient *client, String status, String respType){
  String newLine = "\r\n";
  String response = "";

  response += "HTTP/1.0 " + status + " OK" + newLine;
  response += "Content-Type: " + respType + newLine + newLine;

  for(int i = 0; i < response.length(); i++){
    client->print(response[i]);
  }
}

void ServerObject::sendGetResponseBody(WiFiClient *client, String html){
  for(int i = 0; i < html.length(); i++){
    client->print(html[i]);
  }
}

void ServerObject::setResponse(uint16_t port, String url, Html *response, String method, String respType){
  std::vector<struct Server> *targetServers; 
  
  if(method == METHOD_GET) targetServers = &Servers_get;
  else if(method == METHOD_POST) targetServers = &Servers_post; 
  else return;

  for(int i = 0; i < targetServers->size(); i++){ 
    if(targetServers->at(i).port != port) continue;

    if(targetServers->at(i).findPath(url) >= 0){
      targetServers->at(i).updateResponse(url, response, respType);
    }else{
      targetServers->at(i).setResponse(url, response, respType);
    }
    break;
  }
}

bool ServerObject::removeResponse(uint16_t port, String url, String method){
  std::vector<struct Server> *targetServers;

  if(method == METHOD_GET) targetServers = &Servers_get;
  else if(method == METHOD_POST) targetServers = &Servers_post;
  else return false;

  for(int i = 0; i < targetServers->size(); i++){
    if(targetServers->at(i).port == port){
      return Servers_get[i].removeResponse(utils->fixPath(url));
    }
  }
  return false;
}

void ServerObject::setNotFound(String resp){
  notFoundResp = resp;
}
