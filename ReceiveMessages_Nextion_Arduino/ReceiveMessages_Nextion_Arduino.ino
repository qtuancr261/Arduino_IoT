/*

Thieu Quang Tuan

This example code is in public domain
*/

#include <SoftwareSerial.h>
#include <Nextion.h>

SoftwareSerial nextion(2, 3); // Nextion TX to pin 2 and RX from pin 3 of Arduino
SoftwareSerial esp8266(4, 5); // ESP TX to pin 4 and RX from pin 5 of Arduino
bool LED_1_State = false;
bool LED_2_State = false;
Nextion myNextion(nextion, 9600); //create a Nextion object named myNextion using the nextion serial port @ 9600bps
String dataFromESP;
// Initialize all serials
void setup() 
{
  Serial.begin(9600);
  myNextion.init();
  esp8266.begin(115200);

}
void listenToEsp8266()
{
  if (esp8266.available()) 
 {
    static int dataCount = 0;
    while(esp8266.available())
    {
        char buffChar = (char) esp8266.read();
        dataFromESP += buffChar;
    }
    dataCount++;
    Serial.print("data " + dataFromESP);
    switch(dataCount % 3)
    {
        case 1:
          //myNextion.setComponentText("t1", "3.580.580");
          break;
        case 2:
          myNextion.setComponentText("t1", dataFromESP);
          break;
        case 0:
          myNextion.setComponentText("t2", dataFromESP);
          break;
    }
    if (dataFromESP == "UNO")
    {
        myNextion.setComponentText("t1", "3.580.580");
        myNextion.setComponentText("t2", "17.000,05");
    }
        
    dataFromESP = "";
 }
}
void loop() {
  /*String message = myNextion.listen(); //check for message
  if(message != ""){ // if a message is received...
    if (message == "65 0 1 1 ffff ffff ffff")
        {
          Serial.println("LED 1 button was pressed");
          //myNextion.sendCommand("b0.picc=2");
          //myNextion.sendCommand("ref b0");
          myNextion.buttonToggle(LED_1_State, "b0", 0, 2);
          myNextion.setComponentText("t0", "LED 1");
        }
    else if (message == "65 0 2 1 ffff ffff ffff")
        {
          Serial.println("LED 2 button was pressed");
          //myNextion.sendCommand("b1.picc=2");
          //myNextion.sendCommand("ref b1");
          myNextion.buttonToggle(LED_2_State, "b1", 0, 2);
          myNextion.setComponentText("t0", "LED 2");
        }
    else if (message == "65 0 3 1 ffff ffff ffff")
        {
          Serial.println("Bitcoin Page is being displayed");
          myNextion.setComponentText("t1", "3.580.580");
          myNextion.setComponentText("t2", "17.000,05");
        }
  }*/

  listenToEsp8266();
}
