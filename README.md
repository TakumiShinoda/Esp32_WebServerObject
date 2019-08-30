# Esp32_WebServerObject

## Not Support(will be work)
  * POST request.

## About callback is fired by request.
  `prev(ChainArray queries, ChainArray requests, String *response, WiFiClient *client)`

  * queries: It is parsed from `URL query` to `ChainArray`
  * requests: Show static key list. 
    * path: Path was accessed.
    * params: Query statement by `String`.
    * port: Port was accessed.
    * method: Request method.
  * response: It will be response body. Editable.
  * client: You can stop connection at this callback.