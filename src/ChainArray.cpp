#include "ChainArray.h"

void ChainArray::add(std::vector<String> keys, std::vector<String> values){
  for(int i = 0; i < keys.size(); i++){
    addElement(keys[i], values[i]);
  }
}

void ChainArray::add(String key, String value){
  addElement(key, value);
}

void ChainArray::remove(String key){
  std::vector<uint8_t> keyFound = vector_find(keys(), key);

  if(keyFound.size() > 0){
    Serial.println(keyFound[0]);
    chainArrays.erase(chainArrays.begin() + keyFound[0]);
    chainArrays.shrink_to_fit();
  }
}

void ChainArray::addElement(String key, String value){
  struct chainArray element;
  std::vector<uint8_t> keyFound = vector_find(keys(), key);

  if(keyFound.size() > 0){
    chainArrays[keyFound[0]].value = value;
  }else{
    element.key = key;
    element.value = value;
    chainArrays.push_back(element);
  }
}

std::vector<String> ChainArray::keys(){
  std::vector<String> result;

  for(int i = 0; i < chainArrays.size(); i++){
    result.push_back(chainArrays[i].key);
  }

  return result;
}

void ChainArray::clear(){
  chainArrays.clear();
}

String ChainArray::get(String key){
  for(int i = 0; i < chainArrays.size(); i++){
    if(chainArrays[i].key == key) return chainArrays[i].value;
  }

  return "";
}

std::vector<uint8_t> ChainArray::vector_find(std::vector<String> target, String str){
  std::vector<uint8_t> result;

  for(int i = 0; i < target.size(); i++){
    if(target[i] == str) result.push_back(i);
  }

  return result;
}