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
// DHT 22
#include "DHT.h"
#define DHTPIN 6 
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);


struct packet_t{
  float humidity;  //4
  float temperature;  //4   
  float heatIndex;  // 4
};  // 12

struct packet_t dataPacket = {
  60.0,23.0,0.0 
};

unsigned char command = 0x00;

void setup()
{
  Serial.begin(115200);
  Serial.println("Start");
  dht.begin();
  Wire.begin(2);  // join i2c bus with address #2

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
  float h = dht.readHumidity();
  dataPacket.humidity = h;
  float t = dht.readTemperature();
  dataPacket.temperature = t;
  float f = dht.readTemperature(true);
  if(!(isnan(h) || isnan(t) || isnan(f))) {
    dataPacket.heatIndex = dht.computeHeatIndex(f, h);
  }
  /*
  dataPacket.time ++;
   dataPacket.temperature += random(3);
   dataPacket.temperature -= random(3);
   */
  // Renvoi des données sur I2C master
  Wire.write((byte*)&dataPacket,12);

  // Debug
  //Serial.println(dataPacket.temperature);

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



