#include "stepper.h"
#include "Arduino.h"
#include "pinConfig.h"

/**************************************************
 * @brief  这是步进电机的初始化函数
 * @param  _dir : 步进电机方向控制引脚
 *         _plu : 脉冲驱动引脚，一个脉冲转动一个细分
 *         _ena : （可选）使能引脚，高电平有效，如不使用则默认上拉
 **************************************************/
stepper::stepper(int _dir, int _plu)
{
  dir = _dir;
  plu = _plu;
  pinMode( dir, OUTPUT );
  pinMode( plu, OUTPUT );
  digitalWrite( dir, HIGH );
  digitalWrite( plu, HIGH );
}
stepper::stepper(int _dir, int _plu, int _ena)
{
  dir = _dir;
  plu = _plu;
  ena = _ena;
  pinMode( dir, OUTPUT );
  pinMode( plu, OUTPUT );
  pinMode( ena, OUTPUT );
  digitalWrite( dir, HIGH );
  digitalWrite( plu, HIGH );
  digitalWrite( ena, LOW );
  counter = 0;
}


void stepper::showCounter()
{
  Serial.print("counter:");
  Serial.println(counter);
}

/**************************************************
 * @brief  步进电机反向函数，使接下来的旋转方向与现在相反
 **************************************************/
void stepper::stepper_back()
{
  DELAY
  digitalWrite( dir, !digitalRead( dir ) );
  DELAY
}

/**************************************************
 * @brief  步进电机向既定方向转动若干个细分
 * @param  _steps : 前进细分数
 * @param  _speed : （可选）前进速度，以当前速度为基准的倍率
 **************************************************/
void stepper::march( int _steps )
{
  if( _steps > 0 )
  {
      counter += _steps;
    for(int i=0; i<=_steps; i++)
    {
      digitalWrite(plu, LOW);
      DELAY
      digitalWrite(plu, HIGH);
      DELAY
    }
  }
  else if( _steps < 0 )
    {
      counter += 2*_steps;
      stepper_back();
      march( -_steps );
      stepper_back();
    }
}
void stepper::march( int _steps, float _speed )
{
  int delaytime = int( DELAYTIME / _speed );
  if( _steps > 0 )
  {
    counter += _steps;
    for(int i=0; i<=_steps; i++)
    {
      digitalWrite(plu, LOW);
      delayMicroseconds( delaytime );
      digitalWrite(plu, HIGH);
      delayMicroseconds( delaytime );
    }
  }
  else if( _steps < 0 )
    {
      counter += 2*_steps; //预补偿
      stepper_back();
      march( -_steps );
      stepper_back();
    }
}

void march2(stepper &_stepperA,int _stepsA, stepper &_stepperB, int _stepsB)
{
   if( _stepsA < 0 )
    {
      _stepperA.counter += 2 * _stepsA;
      _stepperA.stepper_back();
      march2( _stepperA, -_stepsA, _stepperB, _stepsB );
      _stepperA.stepper_back();
    }
   else if( _stepsB < 0 )
    {
      _stepperB.counter += 2 * _stepsB;
      _stepperB.stepper_back();
      march2( _stepperA, _stepsA, _stepperB, -_stepsB );
      _stepperB.stepper_back();
    }
  else if( _stepsA < _stepsB )
  {
      march2(  _stepperB, _stepsB, _stepperA, _stepsA);
  }
  else
  {
    _stepperA.counter += _stepsB;
    _stepperB.counter += _stepsB;
    for(int i=0; i<_stepsB; i++)
    {
      digitalWrite(_stepperA.plu, LOW);
      digitalWrite(_stepperB.plu, LOW);
      DELAY
      digitalWrite(_stepperA.plu, HIGH);
      digitalWrite(_stepperB.plu, HIGH);
      DELAY
    }
    _stepperA.march( _stepsA - _stepsB );
  }
}

void march3(stepper &_stepperA,int _stepsA, stepper &_stepperB, int _stepsB, stepper &_stepperC, int _stepsC)
{
   if( _stepsA < 0 )
    {
      _stepperA.counter += 2 * _stepsA;
      _stepperA.stepper_back();
      march3( _stepperA, -_stepsA, _stepperB, _stepsB, _stepperC, _stepsC );
      _stepperA.stepper_back();
      return;
    }
   else if( _stepsB < 0 )
    {
      _stepperB.counter += 2 * _stepsB;
      _stepperB.stepper_back();
      march3( _stepperA, _stepsA, _stepperB, -_stepsB, _stepperC, _stepsC );
      _stepperB.stepper_back();
    }
    else if( _stepsC < 0 )
    {
      _stepperC.counter += 2 * _stepsC;
      _stepperC.stepper_back();
      march3( _stepperA, _stepsA, _stepperB, _stepsB, _stepperC, -_stepsC );
      _stepperC.stepper_back();
    }
  else if( _stepsA < _stepsB )
  {
      march3( _stepperB, _stepsB, _stepperA, _stepsA, _stepperC, _stepsC);
  }
  else if( _stepsB < _stepsC )
  {
      march3( _stepperA, _stepsA, _stepperC, _stepsC, _stepperB, _stepsB);
  }
  else
  {
    _stepperA.counter += _stepsC;
    _stepperB.counter += _stepsC;
    _stepperC.counter += _stepsC;
    for(int i=0; i<=_stepsC; i++)
    {
      digitalWrite(_stepperA.plu, LOW);
      digitalWrite(_stepperB.plu, LOW);
      digitalWrite(_stepperC.plu, LOW);
      DELAY
      digitalWrite(_stepperA.plu, HIGH);
      digitalWrite(_stepperB.plu, HIGH);
      digitalWrite(_stepperC.plu, HIGH);
      DELAY
    }
    march2( _stepperA, _stepsA-_stepsC, _stepperB, _stepsB-_stepsC);
  }
}



void march3_2s(stepper &_stepperA,int _stepsA, stepper &_stepperB, int _stepsB, stepper &_stepperC, int _stepsC)
{
   if( _stepsA < 0 )
    {
      _stepperA.counter += 2 * _stepsA;
      _stepperA.stepper_back();
      march3( _stepperA, -_stepsA, _stepperB, _stepsB, _stepperC, _stepsC );
      _stepperA.stepper_back();
    }
   else if( _stepsB < 0 )
    {
      _stepperB.counter += 2 * _stepsB;
      _stepperB.stepper_back();
      march3( _stepperA, _stepsA, _stepperB, -_stepsB, _stepperC, _stepsC );
      _stepperB.stepper_back();
    }
    else if( _stepsC < 0 )
    {
      _stepperC.counter += 2 * _stepsC;
      _stepperC.stepper_back();
      march3( _stepperA, _stepsA, _stepperB, _stepsB, _stepperC, -_stepsC );
      _stepperC.stepper_back();
    }
  else
  {
    _stepperA.counter += _stepsC;
    _stepperB.counter += _stepsC;
    _stepperC.counter += _stepsC;
    for(int i=0; i<=max(max(_stepsA,_stepsB),_stepsC); i++)
    {
      if(i<_stepsA)
        digitalWrite(_stepperA.plu, LOW);
      if(i<_stepsB)
        digitalWrite(_stepperB.plu, LOW);
      if(i<_stepsC)
        digitalWrite(_stepperC.plu, LOW);
      DELAY
      if(i<_stepsC)
        digitalWrite(_stepperC.plu, HIGH);
      DELAY
      if(i<_stepsA)
        digitalWrite(_stepperA.plu, HIGH);
      if(i<_stepsB)
        digitalWrite(_stepperB.plu, HIGH);
      if(i<_stepsC)
        digitalWrite(_stepperC.plu, LOW);
      DELAY
      if(i<_stepsC)
        digitalWrite(_stepperC.plu, HIGH);
      DELAY
    }
  }
}


void stepper::gohome( void )
{
    march( -counter );
}
    
/**************************************************
 * @brief  关闭步进电机，此时电机不会锁死
 **************************************************/
void stepper::power_off( void )
{
  digitalWrite( ena, LOW );
}

/**************************************************
 * @brief  打开步进电机以进行下一步操作
 **************************************************/
void stepper::power_on( void )
{
  digitalWrite( ena, HIGH );
}
