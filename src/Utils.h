#ifndef Utils_h
#define Utils_h

#include <Arduino.h>
#include <HTTPClient.h>
#include <array>
#include <vector>
#include <regex>
#include <string>
#include "ChainArray.h"

class StatusGen{
  private:
    ChainArray States;

  public:
    void addStatus(int8_t code, String mes);
    void removeStatus(int8_t code);
    String getMes(int8_t code);
    String getArranged(int8_t code, String sep);
};

class Utils{
  public:
    ChainArray analyzeRequestLine(String request);
    String split(String target, char sep, uint8_t index);
    ChainArray analyzeQuery(String str);
    std::vector<uint8_t> vector_find(std::vector<String> target, String str);
    String ints2utf8(std::vector<uint8_t> data);
    String fixPath(String path);
    bool checkFormat(std::string target, char c, std::vector<bool> rule);
    String decodeUrl(String input);
    String getMAC();
    void debugPrint(String head, String message);
};

#endif