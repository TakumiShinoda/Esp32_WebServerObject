#include "Html.h"

Html::Html(String html, void (*prev)(ChainArray, ChainArray, String*, WiFiClient*)){
  htmlObj.html = html;
  htmlObj.prev = prev;
}

String Html::getHtml(){
  return htmlObj.html;
}
