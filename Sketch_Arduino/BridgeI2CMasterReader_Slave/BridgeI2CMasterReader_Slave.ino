/*
 *
 * Hackathon Montpellier 2014 IoT  - Team IoTize
 * Samuel Wald
 *
 * Bridge I2C Master Reader - Sketch Master
 *
 * Module Plug & Play -> Microcontrolleur dans chaque module pour bus general I2C
 * Board Principale (TI LunchPad) -I2C-Principal-> Level Shifter [3V3->5V] -> Arduino Nano -> I2C-Module
 *
 * 2 Arduino connectées en I2C / Master Reader : http://arduino.cc/en/Tutorial/MasterReader
 *
 * Sketch Master
 * Hardware : Arduino Pro Micro BLUE / COM 11
 * 
 * Pins : 
 * Nano :
 * SDA  A4  Orange  
 * SCL  A5  Vert
 *
 * Pro Micro :
 * SDA  2  Orange
 * SCL  3  Vert
 *
 */

#include <Wire.h>

struct packet_t{
  unsigned long time;  //4
  float temperature;  //4   
};  // 8

struct packet_t dataPacket = {
  0,23.0 
};

unsigned char command = 0x00;

void setup()
{
  Serial.begin(115200);
  Serial.println("Start");
  Wire.begin(1);  // join i2c bus with address #2

  // Init
}

void loop()
{
  //Serial.println("Loop");
  Wire.onRequest(requestEvent); // register event Capteur
  Wire.onReceive(receiveEvent); // register event Actionneur
  delay(10);
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent()
{
  // Mesure
  dataPacket.time ++;
  dataPacket.temperature += random(3);
  dataPacket.temperature -= random(3);

  // Renvoi des données sur I2C master
  Wire.write((byte*)&dataPacket,8);

  // Debug
  Serial.println(dataPacket.time);

}

void receiveEvent(int howMany)
{
  while(Wire.available())
  {
    command = Wire.read();
  }
  Serial.print("Command : ");
  Serial.println(command);
}


