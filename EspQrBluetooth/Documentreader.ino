#include <HardwareSerial.h>
#include "BluetoothSerial.h"
#include <EEPROM.h>

#define EEPROM_SIZE           32

#define MOTOR1_RELAY_PIN1     12
//#define MOTOR2_RELAY_PIN1     13
#define MOTOR1_RELAY_PIN2     14
//#define MOTOR2_RELAY_PIN2     15

#define QR_READ1_STATE        1
#define QR_READ2_STATE        2
#define MOTOR_DELAY_STATE     3

#define LED_PIN               33
#define FLASH_LED             4

#define STOP                  0
#define FORWARD               1
#define REVERSE               2

#define TIMEOUT100MS          100
#define TIMEOUT500MS          500
#define TIMEOUT1S             1000
#define TIMEOUT2S             2000
#define TIMEOUT3S             3000
#define TIMEOUT5S             5000

#define DEFAULTBAUDRATE       115200

String device = "DocumentReaderP1";
BluetoothSerial btSerial;
HardwareSerial qrModule(0);

const int rx              = 3;
const int tx              = 1;
String bufrcvQr1;
String rcvQr1;
String rrcvQr1;
String bufrcvQr2;
String rcvQr2;
String rrcvQr2;

String bufrcvBt;
String rcvBt;
String rrcvBt;
String btTransmitted;

const int pagecount       = 100;
int  instantpagecount     = 0;

int btstate               = 0;
int bluetootstate         = 0;
int btRcvDataready        = 0;

int state                 = QR_READ1_STATE;

int qrstate                       = 0;
int bytesRead1DataReady           = 0;
int bytesRead1DataReady2          = 0;

unsigned long SlidingTimeStart    = 0;
unsigned long SlidingTimeEnd      = 0;
unsigned long MotorDelayTime      = 0;
unsigned long BtDelayTime         = 0;

unsigned long MotorTimeoutT1      = 0;
unsigned long MotorTimeoutT2      = 0;

int oneshot = 1;

void setup() {
  Serial.begin(DEFAULTBAUDRATE);
  EEPROM.begin(EEPROM_SIZE);
  instantpagecount =  EEPROM.read(0);
  btSerial.begin(device);
  qrModule.begin(DEFAULTBAUDRATE, SERIAL_8N1, rx, tx); 
  qrModule.available();
  delay(TIMEOUT100MS);   
  btSerial.available();
  delay(TIMEOUT100MS);   
  btSerial.read();
  delay(TIMEOUT100MS);
  btSerial.println("DocumentReader\n\r");
  delay(TIMEOUT100MS);   
  btSerial.read();
  delay(TIMEOUT100MS);   
  btSerial.println("DocumentReader\n\r");
  delay(TIMEOUT100MS);
  pinMode(LED_PIN, OUTPUT);
  pinMode(MOTOR1_RELAY_PIN1, OUTPUT);
  pinMode(MOTOR1_RELAY_PIN2, OUTPUT);
  pinMode(FLASH_LED, OUTPUT);
  rcvQr1.clear();
  rrcvQr1.clear();
  rcvQr2.clear();
  rrcvQr2.clear();
  btTransmitted.clear();
}


void BluetoothFuction() {
  int c=0;

  bluetootstate = btSerial.available();

  if(0 == btstate){
    c = btSerial.read();
    if(c >= 0){      
        //btSerial.println(c);
        //btSerial.flush();
        if(14 == c){
          instantpagecount = 0;
          EEPROM.write(0, 0);
          EEPROM.commit();
        }
        bufrcvBt += (char) c;
        bufrcvBt += btSerial.readStringUntil('\n');
        rcvBt = bufrcvBt;
        bufrcvBt.clear();
        BtDelayTime = millis();             
        btstate = 0;
    }
  }else if(1 == btstate){
    if(millis() -  BtDelayTime > TIMEOUT1S){
        btstate = 2;
    }
  }else if(2 == btstate){    
    //if(1 == btRcvDataready){
       // btRcvDataready = 0;
        btSerial.println(btTransmitted);
        btSerial.flush();
        btTransmitted.clear();
    //}else{
      //  rrcvBt += (char)11;

       // rrcvBt += (char)0;
        //rrcvBt += rcvBt.length();
        //rrcvBt.concat(rcvBt);
       // btSerial.println(rrcvBt);
      //  btSerial.flush();
    //}
     // rcvBt.clear();
    //  rrcvBt.clear();
    
  }

}

void QRReadFuction() {
  int c=0;
    if(qrstate == 0){
      c = qrModule.read();
      if(c >= 0){
        bufrcvQr1 += (char) c;
        bufrcvQr1 += qrModule.readStringUntil('\r');
          rcvQr1 = bufrcvQr1;
          bufrcvQr1.clear();
          qrModule.flush();      
          bytesRead1DataReady = 1;
          qrstate = 1;
      }
    }else if(qrstate == 1){
      c = qrModule.read();
      if(c >= 0){
        bufrcvQr2 += (char) c;
        bufrcvQr2 += qrModule.readStringUntil('\r');
        rcvQr2 = bufrcvQr2;
        bufrcvQr2.clear();
        qrModule.flush();
        bytesRead1DataReady2 = 1;
        qrstate = 0;
      }
    }
}

void MotorRun(uint8_t StartStop){
  if(FORWARD == StartStop){
    digitalWrite(MOTOR1_RELAY_PIN1, HIGH);
    digitalWrite(MOTOR1_RELAY_PIN2, LOW);
    MotorTimeoutT1 = millis();
  }else if(STOP == StartStop){
    digitalWrite(MOTOR1_RELAY_PIN1, LOW);
    digitalWrite(MOTOR1_RELAY_PIN2, LOW);
    MotorTimeoutT1 = 0;
  }else  if(REVERSE == StartStop){
    digitalWrite(MOTOR1_RELAY_PIN1, LOW);
    digitalWrite(MOTOR1_RELAY_PIN2, HIGH);
    MotorTimeoutT1 = millis();
  }else{
    //TODO: Return error code
  }
}
void MotorRunTimeout(){
  if( millis() - MotorTimeoutT1 > TIMEOUT3S && MotorTimeoutT1 != 0){
      MotorRun(STOP);
      qrstate = 0;
      state = QR_READ1_STATE;
  }
}

void loop() {
  QRReadFuction();
  BluetoothFuction();
  MotorRunTimeout();

  if (state == QR_READ1_STATE) {
    if (bytesRead1DataReady == 1) {   
      bytesRead1DataReady = 0;      
      //btSerial.print("ReceivedQr1: ");
      rrcvQr1 += (char)11;
      rrcvQr1 += (char)rcvQr1.length();
      //btSerial.print("QR1 Lenght:");
      //btSerial.println(rcvQr1.length());
      rrcvQr1.concat(rcvQr1);
      //btSerial.println(rrcvQr1);
      btTransmitted = rrcvQr1;
      rcvQr1.clear();
      rrcvQr1.clear();  
      if (instantpagecount < pagecount){
        MotorRun(FORWARD);
        SlidingTimeStart = millis();
        state = QR_READ2_STATE;
      }else{
        qrstate = 0;
      }
      
    } 
  }else if (state == QR_READ2_STATE) {
    if (bytesRead1DataReady2 == 1) { 
      bytesRead1DataReady2 = 0;
      rrcvQr2 += (char)11;
      rrcvQr2 += (char)rcvQr2.length();
      //btSerial.print("QR2 Lenght:");
      //btSerial.println(rcvQr2.length());
      rrcvQr2.concat(rcvQr2);
      //btSerial.println(rrcvQr2);
      rcvQr2.clear();
      rrcvQr2.clear();

      SlidingTimeEnd = millis();
      //btSerial.print("Timing1: ");
      //btSerial.println(SlidingTimeEnd -  SlidingTimeStart);
      //btSerial.flush();
      state = MOTOR_DELAY_STATE;
    }
  }else if (state == MOTOR_DELAY_STATE){
    MotorDelayTime = millis() - SlidingTimeEnd;    
    if(MotorDelayTime > TIMEOUT500MS){
      //btSerial.print("Timing2: ");
      //btSerial.println(MotorDelayTime);
      //btSerial.flush();
      btSerial.println(btTransmitted);
      btSerial.flush();
      btTransmitted.clear();
      btRcvDataready = 1;
      MotorRun(STOP);
      EEPROM.write(0, ++instantpagecount);
      EEPROM.commit();
      state = QR_READ1_STATE;
    }        
  }   
}

