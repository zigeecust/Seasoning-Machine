

#define PLU_Y_UP LOW

#define DELAY DELAY600
#define DELAYTIME 600

#define DELAY1000 delayMicroseconds(1000);
#define DELAY900  delayMicroseconds(900);
#define DELAY600  delayMicroseconds(600);
#define DELAY300  delayMicroseconds(300);

void march(int x, int y);

class stepper
{
  public:
    stepper(int _dir, int _plu);
    stepper(int _dir, int _plu, int _ena);
    void stepper_back( void );
    void march( int _steps );
    void march( int _steps, float _speed );
    void gohome( void );
    void power_off();
    void power_on();
    void showCounter();
    friend void march2(stepper &_stepperA,int _stepsA, stepper &_stepperB, int _stepsB);
    friend void march3(stepper &_stepperA,int _stepsA, stepper &_stepperB, int _stepsB, stepper &_stepperC, int _stepsC);
    friend void march3_2s(stepper &_stepperA,int _stepsA, stepper &_stepperB, int _stepsB, stepper &_stepperC, int _stepsC);
    int counter;
  private:
    int plu, dir, ena;
};


