#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>      
#include <Nextion.h>


const char* ssid = "RedmiTQT";
//const char* ssid = "hoianhtung";
//const char* ssid = "DrayTek-LAN-A";
const char* password = "261261261";
//const char* password = "lagikhongbietdau";
const char* host = "tungtu-btc.herokuapp.com";
const int httpPort = 80;
unsigned long previousMillis = 0;
const long interval = 3000;

SoftwareSerial nextion(D6, D7);// Nextion TX to pin 2 and RX to pin 3 of Arduino
Nextion myNextion(nextion, 9600); //create a Nextion object named myNextion using the nextion serial port @ 9600bps

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

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


void setup() {
  Serial.begin(9600);
  delay(10);
  
  // Setup alert LEDs here
  
  Serial.println("Connect to ");
  Serial.print(ssid);

  //Setup network
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  //Connecting
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  //Show us that ESP has connected to the network we provided
  Serial.println();
  Serial.print("Connected at ");
  Serial.print(WiFi.localIP());
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
  String time = root["results"]["time"]["time_update"];
  Serial.println("Time updated: " + time);
  String usdRate = root["results"]["bpi"]["USD"]["rate"];
  Serial.println("USD: " + usdRate +"$");
  
  String vnRate = root["results"]["bpi"]["VND"]["rate"];
  Serial.println("VND: " + vnRate);
  myNextion.setComponentText("t1", vnRate);
  delay(2000);
  myNextion.setComponentText("t2", usdRate +"$");
  /*String data;
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
  Serial.println("-----------------");
  Serial.println("RATE : ");
  //Serial.println(data);
  //data.trim();

  int rateIndex = data.indexOf("rate");
  String coinPrice = data.substring(rateIndex + 7, rateIndex + 15);
  coinPrice.trim();
  Serial.println("Price in USD: " + coinPrice);
  myNextion.setComponentText("t2", coinPrice);
  //myNextion.setComponentText("t2", "17.000");

  rateIndex = data.lastIndexOf("rate");
  coinPrice = data.substring(rateIndex + 7, rateIndex + 18);
  coinPrice.trim();
  Serial.println("Price in VND: " + coinPrice);

  int dateIndex = data.indexOf("date_update");
  String dateUpdated = data.substring(dateIndex + 14, dateIndex + 24);
  dateUpdated.trim();

  int timeIndex = data.indexOf("time_update");
  String timeUpdated = data.substring(timeIndex + 14, timeIndex + 22);
  timeUpdated.trim();
  
  Serial.println("Time updated: " + dateUpdated + " - " + timeUpdated);*/
}
void loop() {
  String APIUrl = "/api/btc";
  //getBitcoinInformation(APIUrl);
  String message = myNextion.listen(); //check for message
  if(message != ""){ // if a message is received...
    if (message == "65 0 3 1 ff ff ff")
        {
          Serial.println("Bitcoin Page is being displayed");
          getBitcoinInformation(APIUrl);
          //myNextion.setComponentText("t1", "3.500.000");
          //myNextion.setComponentText("t2", "17.000");
        }
        Serial.print(message);
  }
}
