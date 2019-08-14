


#include "stepper.h"
#include "pinConfig.h"
#include "process.h"

#include <Servo.h>

extern int gain[7];
unsigned char recebuff[2];
extern int goout;

stepper step_a( DIRa, PLUa, ENAa);
stepper step_b( DIRb, PLUb, ENAb);
stepper step_c( DIRc, PLUc, ENAc);
stepper step_d( DIRd, PLUd, ENAd);

Servo myservo; 

void setup()
{
  pinMode(BeepPin, OUTPUT);
  pinMode(motor1, OUTPUT);
  pinMode(motor2, OUTPUT);
  pinMode(motor3, OUTPUT);
  pinMode(motor4, OUTPUT);
  pinMode(magnet1,OUTPUT);
  pinMode(magnet2,OUTPUT);
  pinMode(magnet3,OUTPUT);
  pinMode(magnet4,OUTPUT);

digitalWrite(BeepPin,0);
digitalWrite(motor1,0);
digitalWrite(motor2,0);
digitalWrite(motor3,0);
digitalWrite(motor4,0);
digitalWrite(magnet1,0);
digitalWrite(magnet2,0);
digitalWrite(magnet3,0);
digitalWrite(magnet4,0);

 myservo.attach(41); 
  beep();
  Serial.begin(115200);
  Serial1.begin(9600);
  Serial2.begin(9600);

  pinMode(LIMc,INPUT);
  step_c.stepper_back();
  while(1) 
  {
    step_c.march(1);
    delay(2);
    if(!digitalRead(LIMc))
      break;
  }
  step_c.stepper_back();
  step_c.counter = 0;
  delay(500);


  pinMode(LIMa,INPUT);
  step_a.stepper_back();
  while(1) 
  {
    step_a.march(1);
    delay(2);
    if(!digitalRead(LIMa))
      break;
  }
  step_a.stepper_back();
  step_a.counter = 0;
  
}

void loop()
{
  while(1)
  {
    if(Serial1.available())
    {
      unsigned char ch = Serial1.readBytes(recebuff,2);
      switch(recebuff[0])
      {
        beep();
        case 0x10: //菜谱
          Serial.println("cook book mode.");
          cookbook(recebuff[1]);
          break;
        case 0x20: case 0x21: case 0x22: case 0x23: case 0x24: case 0x25: case 0x26: //口味
          gain[recebuff[0]-0x20] = recebuff[1];
          break;
        case 0x30: case 0x31: case 0x32: case 0x33: //液体软手操
          ProcessWater(recebuff[0]-0x30, recebuff[1]);
          break;
        case 0x34: case 0x35: //固体软手操
          ProcessSolid(recebuff[0]-0x34, recebuff[1]);
          break;
        case 0x36: 
          ProcessSolidServo();
          break;
        case 0x40: case 0x41: case 0x42: case 0x43: //液体上料软手操
          LoadingWater(recebuff[0]-0x40, recebuff[1]);
          break;
        case 0x44: case 0x45: case 0x46:  //固体上料软手操
          LoadingSolid(recebuff[0]-0x44, recebuff[1]);
          break;
       
         
        case 0x50: case 0x51: case 0x52: case 0x53:
          hardhandling(recebuff[0]-0x50, recebuff[1]);
          break;
        case 0x60:
          goout = recebuff[1];
        default:
          break;
      }
    }
  }
}


