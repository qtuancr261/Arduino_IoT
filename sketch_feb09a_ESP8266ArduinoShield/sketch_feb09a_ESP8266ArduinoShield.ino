
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include <Nextion.h>
const char* ssid     = "RedmiTQT";
//const char* ssid     = "hoianhtung";
const char* password = "261261261";
//const char* password = "lagikhongbietdau";
const char* host = "tungtu-btc.herokuapp.com";
const int httpPort = 80;
const int led_red = 4;
const int led_green = 5;
const int led_yellow = 16;
SoftwareSerial nextion(12, 14);
Nextion myNextion(nextion, 9600); 
//---------------------------------------------------------------


String getResponse( String url) 
{

   WiFiClient client;
  if (!client.connect(host, httpPort)) {
    return "failed";
  }
    //Send request
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
//---------------------------------------------------------------
void getBitcoinInformation(String APIUrl)
{
  String replyFromServer = getResponse(APIUrl);
  //Serial.print(replyFromServer);
  if (replyFromServer == "failed")
    return;
  
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
  
  data = replyFromServer.substring(jsonIndex);


  DynamicJsonBuffer jsonBuffer;
 
  JsonObject& root = jsonBuffer.parseObject(data);
  
  Serial.print("s");
  
  digitalWrite(led_yellow, HIGH);
  
  delay(500);
  String time = root["results"]["time"]["time_update"];
  String date = root["results"]["time"]["date_update"];
  Serial.print(time + "  " + date);
  myNextion.setComponentText("g2", time + "  " + date);
  digitalWrite(led_yellow, LOW);
  delay(500);
  // Get USD rate
  String usdRate = root["results"]["bpi"]["USD"]["rate"];
  double usd = root["results"]["bpi"]["USD"]["rate_float"];
  Serial.print(usdRate +"$");
  myNextion.setComponentText("t2", usdRate +"$");
  digitalWrite(led_yellow, HIGH);
  //Serial.println(usd);
  delay(500);
  // Get VN rate
  String vnRate = root["results"]["bpi"]["VND"]["rate"];
  double vnd = root["results"]["bpi"]["VND"]["rate_float"];
  Serial.print(vnRate);
  myNextion.setComponentText("t1", vnRate);
  digitalWrite(led_yellow, LOW);
  delay(500);
  alertLEDs(usd);
}
 //-----------------------------------------------------------------------------
 void alertLEDs(double currentRate)
 {
    static double preRate = currentRate;
    if (currentRate > preRate)
        {
          digitalWrite(led_green, HIGH);
          delay(50);
          digitalWrite(led_green, LOW);
          delay(50);
          digitalWrite(led_green, HIGH);
          delay(50);
          digitalWrite(led_green, LOW);
          delay(50);
        }
    else if (currentRate < preRate)
        {
          digitalWrite(led_red, HIGH);
          delay(50);
          digitalWrite(led_red, LOW);
          delay(50);
          digitalWrite(led_red, HIGH);
          delay(50);
          digitalWrite(led_red, LOW);
          delay(50);
        }
    preRate = currentRate;   
 }
//------------------------------------------------------------------------------

void setup() {
  
 Serial.begin(115200);    
 WiFi.disconnect(); 
 // Bắt đầu kết nối 
 WiFi.mode(WIFI_STA);  
 WiFi.begin(ssid, password); // Connect to WIFI network
 // Setup LEDs pin mode 
 pinMode(led_red, OUTPUT);
 pinMode(led_green, OUTPUT);
 pinMode(led_yellow, OUTPUT);
// Wait for connection
 while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.println(".");
 }
 Serial.print("Connected to ");
 Serial.println(ssid);
 Serial.print("IP address: ");
 Serial.println(WiFi.localIP());
 myNextion.init();
 //digitalWrite(led_green, HIGH);
 //digitalWrite(led_red, HIGH);
}

void loop() {
 // Api url 
 String APIUrl = "/api/btc";
 getBitcoinInformation(APIUrl);
 delay(1000);
}
