
#include "stepper.h"
#include "pinConfig.h"
#include "process.h"
#include "Arduino.h"
#include <Servo.h>


extern Servo myservo; 
extern stepper step_a;
extern stepper step_b;
extern stepper step_c;
extern stepper step_d;

int s1[16] = {46, 131  , 201 ,  245  , 249  , 209   ,119  ,   0  ,-119,  -209 , -249 , -245 , -201  ,-131  , -46};
int s2[16] = {368,   197,    35,   -65,  -123 , -154 , -170  ,-175  ,-170  ,-154  ,-123,   -66    ,36  , 197  , 367};

int goout = 1;

void beep()
{
  delay(500);
  digitalWrite(BeepPin, HIGH);
  delay(500);
  digitalWrite(BeepPin, LOW);
  delay(500);
  digitalWrite(BeepPin, HIGH);
  delay(500);
  digitalWrite(BeepPin, LOW);
}


int gain[7] = {50, 50, 50, 50, 50, 50, 50};
int book [10][7] = {
  {50, 50, 50, 50, 50, 50, 50}, // 红烧肉
  {50, 50, 50, 50, 50, 50, 50}, // 可乐鸡翅
  {50, 50, 50, 50, 50, 50, 50}, // 糖醋排骨
  {50, 50, 50, 50, 50, 50, 50}, // 鱼香肉丝
  {50, 50, 50, 50, 50, 50, 50}, // 油焖大虾
  {50, 50, 50, 50, 50, 50, 50}, // 麻婆豆腐
  {50, 50, 50, 50, 50, 50, 50}, // 干煸花菜
  {50, 50, 50, 50, 50, 50, 50}, // 蜜汁鸡翅
  {50, 50, 50, 50, 50, 50, 50}, // 番茄炒蛋
  {50, 50, 50, 50, 50, 50, 50}  // 酸辣土豆丝
};

void cookbook(int id)
{
  int level=0, i=0;
 
  step_c.march(200);
  step_a.march(waterout);
  delay(1000);

  level = -2 * book[id][i] * gain[i] / 5;
  drawCircle16(level);
  step_c.march(200); step_d.gohome(); step_c.march(200);
  i++;
  level = -2 * book[id][i] * gain[i] / 5;
  drawCircle16(level);
  step_c.march(200); step_d.gohome(); step_c.march(200);
  i++;
  level = -2 * book[id][i] * gain[i] / 5;
  drawCircle16(level);
  step_c.march(200); step_d.gohome(); step_c.march(200);
  i++;
  level = -2 * book[id][i] * gain[i] / 5;
  drawCircle16(level);
  step_d.gohome(); step_c.gohome();

  step_a.march(400-waterout);
 
  int countTime = 0;
  int ammount =  book[id][4] * gain[4] / 50 / 20;
  int a,b,c;
  a=magnet1; b=magnet2;c=motor1;
  
  digitalWrite(c,1);
  for(int i=0; i<5; i++)
  {
    if(countTime < ammount) digitalWrite(a, 1);
    march2(step_a,  s1[i*3  ], step_b, s2[i*3  ]);
    if(countTime < ammount) { digitalWrite(a, 0); digitalWrite(b, 1); }
    march2(step_a,  s1[i*3+1], step_b, s2[i*3+1]);
    if(countTime < ammount) digitalWrite(b, 0); 
    march2(step_a,  s1[i*3+2], step_b, s2[i*3+2]);
    countTime ++;   
  }
  digitalWrite(c,0);
  march2(step_a,  s1[15], step_b, s2[15]);
  delay(1000);

  ammount =  book[id][4] * gain[4] / 50 / 20;
  a=magnet3; b=magnet4; c=motor2;
  digitalWrite(c,1);
  for(int i=0; i<5; i++)
  {
    if(countTime < ammount) digitalWrite(a, 1);
    march2(step_a,  s1[i*3  ], step_b, s2[i*3  ]);
    if(countTime < ammount) { digitalWrite(a, 0); digitalWrite(b, 1); }
    march2(step_a,  s1[i*3+1], step_b, s2[i*3+1]);
    if(countTime < ammount) digitalWrite(b, 0); 
    march2(step_a,  s1[i*3+2], step_b, s2[i*3+2]);
    countTime ++;   
  }
  digitalWrite(c,0);
  march2(step_a,  s1[15], step_b, s2[15]);
  delay(1000);

  int pos;  
  digitalWrite(motor3,1);
  for(int i=0; i<8; i++)
  {
    for (pos = 0; pos <= 180; pos += 1)
    { myservo.write(pos); delay(1);}
    march2(step_a,  s1[i*2  ], step_b, s2[i*2  ]);
    for (pos = 180; pos >= 0; pos -= 1)
    { myservo.write(pos); delay(1);}
    march2(step_a,  s1[i*2+1], step_b, s2[i*2+1]);
  }
  digitalWrite(motor3,0);
  
  delay(1000);  
  step_a.gohome();
  step_b.gohome();
  step_c.gohome();
  step_d.gohome();
}

// kind = 0,1,2,3
void ProcessWater(unsigned char kind, unsigned char ammount)
{
  int level = -20 * ammount;
  step_c.march(200+400*kind);

  if((goout==0))
    step_d.march(level);
  else
  {
    step_a.march(waterout);
    delay(1000);
    drawCircle16(level);
  }
  
  delay(1000);
  step_a.gohome();
  step_b.gohome();
  step_c.gohome();
  step_d.gohome();
}

//装载液体
void LoadingWater(unsigned char kind, unsigned char ammount)
{
  int level = 100 * ammount;
  step_c.march(200+400*kind);
  delay(1000);
  step_d.march(level);
  delay(1000);
  step_c.gohome();
  step_d.gohome();
}

int getRedout()
{
  if(digitalRead(redOut)==LOW)
  {
    delay(80);
    if(digitalRead(redOut)==LOW)
      return 1;
  }
  return 0;
}

void LoadingSolid(unsigned char kind, unsigned char ammount)
{
  step_b.march(-1500);
  delay(1000);
  digitalWrite(motor4,1);
  while(1)
  {
    if(!getRedout())
      break;
  }
  digitalWrite(motor4,0);
  delay(1000);
  step_a.gohome();
  step_b.gohome();
  step_c.gohome();
  step_d.gohome();
}

void ProcessSolidServo()
{
  int pos;
  step_a.march(400);
  
  digitalWrite(motor3,1);
  for(int i=0; i<8; i++)
  {
    for (pos = 0; pos <= 180; pos += 1)
    { myservo.write(pos); delay(1);}
    march2(step_a,  s1[i*2  ], step_b, s2[i*2  ]);
    for (pos = 180; pos >= 0; pos -= 1)
    { myservo.write(pos); delay(1);}
    march2(step_a,  s1[i*2+1], step_b, s2[i*2+1]);
  }
  
  digitalWrite(motor3,0);
  delay(1000);
  step_a.gohome();
  step_b.gohome();
  step_c.gohome();
  step_d.gohome();
}

void ProcessSolid(unsigned char kind, unsigned char ammount)
{
  int countTime = 0;
  ammount /= 20;
  step_a.march(400);
  int a,b,c;
  if(kind==0)
  {a=magnet1; b=magnet2;c=motor1;}
  else if(kind==1)
  {a=magnet3; b=magnet4;c=motor2;}
  
  digitalWrite(c,1);
  for(int i=0; i<5; i++)
  {
    if(countTime < ammount) digitalWrite(a, 1);
    march2(step_a,  s1[i*3  ], step_b, s2[i*3  ]);
    if(countTime < ammount) { digitalWrite(a, 0); digitalWrite(b, 1); }
    march2(step_a,  s1[i*3+1], step_b, s2[i*3+1]);
    if(countTime < ammount) digitalWrite(b, 0); 
    march2(step_a,  s1[i*3+2], step_b, s2[i*3+2]);
    countTime ++;   
  }
  digitalWrite(c,0);
  march2(step_a,  s1[15], step_b, s2[15]);
  delay(1000);
  step_a.gohome();
  step_b.gohome();
  step_c.gohome();
  step_d.gohome();
  /*
  if(kind == 0)
  {
    for(int i; i<ammount; i++)
    {
      digitalWrite(magnet1, 1);
      delay(1400);
      digitalWrite(magnet1, 0);
      digitalWrite(magnet2, 1);
      delay(1400);
      digitalWrite(magnet2, 0 );
      delay(1400);
    }
  }
  else if(kind == 1)
  {
    for(int i; i<ammount; i++)
    {
      digitalWrite(magnet3, 1);
      delay(1400);
      digitalWrite(magnet3, 0);
      digitalWrite(magnet4, 1);
      delay(1400);
      digitalWrite(magnet4, 0 );
      delay(1400);
    }
  }*/
}


void drawCircle4(int level)
{
  march3(step_a,  600, step_b, -600, step_d, level/4);
  march3(step_a,  600, step_b,  600, step_d, level/4);
  march3(step_a, -600, step_b,  600, step_d, level/4);
  march3(step_a, -600, step_b, -600, step_d, level/4);
}

int chickBowl()
{
  if(digitalRead(bowlPin)==HIGH)
    return 0;
  else
    return 1;
}

void drawCircle16(int level)
{
    for(int i=0; i<16; i++)
      march3(step_a,  s1[i], step_b, s2[i], step_d, level/16);
}


void hardhandling(unsigned char obj, unsigned char cmd)
{
  //switch()
}





