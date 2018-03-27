#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#define LED D0
const char* ssid     = "RedmiTQT";
const char* password = "261261261";
const char* host = "tungtu-btc.herokuapp.com";
const int httpPort = 80;
ESP8266WebServer server(80); // HTTP server on port 80
//---------------------------------------------------------------
String getResponse( String url) {

   WiFiClient client;

  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return "failed";
  }
    //Send our request
  client.print(String("GET ") + url +" HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return "null";
    }
  }
  String reply = "";
  //Serial.println("Reponse: ");
  while(client.available()){
    String line = client.readStringUntil('\r');
    reply +=line;
  }
  
  return reply;
}
void getBitcoinInformation(String APIUrl)
{
  String replyFromServer = getResponse(APIUrl);
  //Serial.print(replyFromServer);
  String data;
  int jsonIndex = 0;
  for (int i = 0; i < replyFromServer.length(); i++)
  {
    if (replyFromServer[i] == '{')
    {
      jsonIndex = i;
      break;
    }
  }
  // Get data we need
  data = replyFromServer.substring(jsonIndex);
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(data);
  // Get time updated
  String time = root["results"]["time"]["time_update"];
  Serial.println(time);
  delay(1000);
  // Get USD rate
  String usdRate = root["results"]["bpi"]["USD"]["rate"];
  Serial.println(usdRate +"$");
  delay(1000);
  // Get VN rate
  String vnRate = root["results"]["bpi"]["VND"]["rate"];
  Serial.println(vnRate);
  delay(1000);
}
//------------------------------------------------------------------------------
void setup() {
 Serial.begin(115200);    
 WiFi.disconnect(); 
 // Disconnect AP
 WiFi.mode(WIFI_STA);  
 WiFi.begin(ssid, password); // Connect to WIFI network
 pinMode(LED, OUTPUT);
// Wait for connection
 while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.println(".");
 }
 Serial.print("Connected to ");
 Serial.println(ssid);
 Serial.print("IP address: ");
 Serial.println(WiFi.localIP());

  server.on("/", [](){
  server.send(200, "text/plain", "Hello KDE Neon - This ESP8266 Wifi Shield");
 });

  server.begin(); // Start HTTP server
  Serial.println("HTTP server started.");
}

void loop() {
 server.handleClient();
 String APIUrl = "/api/btc";
 getBitcoinInformation(APIUrl);
 delay(1000);
}
