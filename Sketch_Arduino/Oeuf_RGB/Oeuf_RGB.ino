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

#define GREEN 8
#define BLUE 9
#define RED 7
#define delayTime 20

struct packet_t{
  unsigned char commande;  //4   Unused
};  // 4

struct packet_t dataPacket = {
  0 
};

unsigned char command = 0x00;

void setup()
{
  Serial.begin(115200);

  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(RED, OUTPUT);
  digitalWrite(GREEN, HIGH);
  digitalWrite(BLUE, HIGH);
  digitalWrite(RED, HIGH);

  Serial.println("Start");

  Wire.begin(3);  // join i2c bus with address #2

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
  /*
  dataPacket.time ++;
   dataPacket.temperature += random(3);
   dataPacket.temperature -= random(3);
   
   // Renvoi des données sur I2C master
   Wire.write((byte*)&dataPacket,8);
   */
  // Debug
  //Serial.println(dataPacket.time);

}

void receiveEvent(int howMany)
{
  while(Wire.available())
  {
    command = (unsigned char) Wire.read();
  }
  Serial.print("Command : ");
  Serial.println(command);

  switch(command){
  case 0x01 : 
    RGB(0,0,0);
    break;
  case 0x02 : 
    RGB(255,0,0);
    break;
  case 0x04 : 
    RGB(0,255,0);
    break;
  case 0x08 : 
    RGB(0,0,255);
    break;
  case 0x0A : 
    RGB(120,120,120);
    break;
  case 0x10 : 
    RGB(255,255,0);
    break;
  case 0x1A : 
    RGB(255,0,255);
    break;
  case 0x20 : 
    RGB(0,255,255);
    break;
  case 0x2A : 
    RGB(255,120,0);
    break;
  case 0x30 : 
    RGB(255,0,120);
    break;
  case 0x3A : 
    RGB(0,255,120);
    break;
  case 0x40 : 
    RGB(120,255,0);
    break;
  case 0x4A : 
    RGB(0,120,255);
    break;
  case 0x50 : 
    RGB(120,0,255);
    break;
  case 0x5A : 
    RGB(255,255,255);
    break;
  default : 
    break;
  }
}

void RGB(int r,int g,int b){
  analogWrite(RED,r);
  analogWrite(GREEN,g);
  analogWrite(BLUE,b);
}



