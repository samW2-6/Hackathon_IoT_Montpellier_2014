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
 * Hardware : Arduino Nano 3 RED / COM 10
 *
 * Pins : 
 * SDA  A4  Orange  
 * SCL  A5  Vert
 *
 * Si LunchPad Tiva C TM4C1294XL  http://www.ti.com/lit/ug/spmu365a/spmu365a.pdf  Table 2.1
 *  Port COM 14
 *  GND  Proche Micro USB DEBUG
 *  SDA  PB3  Orange
 *  SCL  PB2  Vert
 *
 */

#include <Wire.h>

//#define TILunchPad

// Data c1  __________________________  Conserve pour test precedent
struct packetC1_s{  // s pour structure
  unsigned long time;  //4
  float temperature;  //4   
};  // 8
typedef union unionC1{
  struct packetC1_s data;  // 8
  byte by[sizeof(struct packetC1_s)];
};
union unionC1 unionC1_i;  // i pour instance

// Data c2  ___________________________  Oeuf_DHT22
struct packetC2_s{  // s pour structure
  float humidity;  //4
  float temperature;  //4   
  float heatIndex;  //4
};  // 12
typedef union unionC2{
  struct packetC2_s data;  // 12
  byte by[sizeof(struct packetC2_s)];
};
union unionC2 unionC2_i;  // i pour instance

// Data c3  ___________________________
struct packetC3_s{  // s pour structure
  unsigned long time;  //4
  float temperature;  //4   
};  // 8
typedef union unionC3{
  struct packetC3_s data;  // 8
  byte by[sizeof(struct packetC3_s)];
};
union unionC3 unionC3_i;  // i pour instance

// Data c4  ___________________________
struct packetC4_s{  // s pour structure
  unsigned long time;  //4
  float temperature;  //4   
};  // 8
typedef union unionC4{
  struct packetC4_s data;  // 8
  byte by[sizeof(struct packetC4_s)];
};
union unionC4 unionC4_i;  // i pour instance

// Data c5  ___________________________
struct packetC5_s{  // s pour structure
  unsigned long time;  //4
  float temperature;  //4   
};  // 8
typedef union unionC5{
  struct packetC5_s data;  // 8
  byte by[sizeof(struct packetC5_s)];
};
union unionC5 unionC5_i;  // i pour instance

// Data a1  ___________________________
struct packetA1_s{  // s pour structure
  unsigned long time;  //4
  float temperature;  //4   
};  // 8
typedef union unionA1{
  struct packetA1_s data;  // 8
  byte by[sizeof(struct packetA1_s)];
};
union unionA1 unionA1_i;  // i pour instance

//  _____________________

//long int dataR = 0;
boolean c1,c2,c3,c4,c5,a1;  // flags de connection des capteurs/actionneurs

int payC1 = (int) sizeof(struct packetC1_s);
int payC2 = (int) sizeof(struct packetC2_s);
int payC3 = (int) sizeof(struct packetC3_s);
int payC4 = (int) sizeof(struct packetC4_s);
int payC5 = (int) sizeof(struct packetC5_s);
int payA1 = (int) sizeof(struct packetA1_s);

//definition des addresses I2C de chaque module
int addC1 = 1;
int addC2 = 2;
int addC3 = 3;
int addC4 = 4;
int addC5 = 5;
int adA1  = 6;


unsigned char commandA1 = 0X00;
int delayTime = 200;
unsigned long lastTimeC2 = 0;

void setup()
{
  c1=c2=c3=c4=c5=a1= false;

#ifdef TILunchPad
  Wire.setModule(0);
#endif

  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(115200);  // start serial for output
  Serial.println("Start");
  c1 = a1 = true;
  Serial.print("Number of devices connected : ");
  Serial.println(scanI2C());
  // init connections, delayTime, etc

}

void loop()
{
  // Mesure des capteurs connectes & activation des actionneurs connectes
  requestData(addC1,payC1);
  for(int i=0;i < payC1; i++){
    unionC1_i.by[i] = Wire.read(); 
  }

  if(c2){
    if((millis() - lastTimeC2) > 2000){
      lastTimeC2 = millis();
      requestData(addC2,payC2);
      for(int i=0;i < payC2; i++){
        unionC2_i.by[i] = Wire.read(); 
      }
    }
  }
  if(c3){
    requestData(addC3,payC3);
    for(int i=0;i < payC3; i++){
      unionC3_i.by[i] = Wire.read(); 
    }
  }
  if(c4){
    requestData(addC4,payC4);
    for(int i=0;i < payC4; i++){
      unionC4_i.by[i] = Wire.read(); 
    }
  }
  if(c5){
    requestData(addC5,payC5);
    for(int i=0;i < payC5; i++){
      unionC5_i.by[i] = Wire.read(); 
    }
  }
  if(a1){  // Envoi d'une commande a un actionneur
    Wire.beginTransmission(4); // transmit to device #4
    commandA1++;
    Wire.write(commandA1);              // sends one byte  
    Wire.endTransmission();    // stop transmitting
  }


  // Envoi des données accessibles sous forme de structures
  if(c1){
    Serial.print(unionC1_i.data.time);
    Serial.print("\t");
    Serial.println(unionC1_i.data.temperature);
  }

  if(c2){
    //envoi des données de C2 
  }
  if(c3){
    //envoi des données de C2
  }

  delay(delayTime);
  //resetScanI2C();
}

//  Demande au module d'adresse I2C [addr] une donnée de longueur [payload] octets, stockee dans le tableau d'octets [data].
//  Cette fonction modifie un tableau d'octets qui sera ensuite affecté à une variable union pour recupere les données intrinseques.
//  Parametres :
//  (in)  addr      Adresse du module
//  (in)  payload   Taille en octet de la donnée demandée 
void requestData(int addr, int payload){
  Wire.requestFrom(addr, payload); 
  while(Wire.available() < payload )    // wait for all the [payload] bytes
  {
  }
}

int scanI2C(){
  byte error, address;
  int nDevices;

  nDevices = 0;
  for(address = 1; address < 127; address++ ) 
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      switch(address){
      case 1 : 
        c1 = true;
        break;
      case 2 : 
        c2 = true;
        break;
      case 3 : 
        c3 = true;
        break;
      case 4 : 
        c4 = true;
        break;
      case 5 : 
        c5 = true;
        break;
      case 6 : 
        a1 = true;
        break;
      default : 
        break;
      }
      nDevices++;
    }
  }
  return nDevices;
}

void resetScanI2C(){
  c1=c2=c3=c4=c5=a1= false; 
}





