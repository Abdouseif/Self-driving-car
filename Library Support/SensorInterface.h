/// This library installs three ultrasonic sensors using:
// Timer1 A with pins : PB0 trigger, PB4 Echo
// Timer1 B with pins : PB1 trigger, PB5 Echo
// Timer0 A with pins : PB2 trigger, PB6 Echo

void InitSensor_04(void);

double MeasureDistance_04(void);


void InitSensor_15(void);

double MeasureDistance_15(void);

void InitSensor_26(void);

double MeasureDistance_26(void);

void SysTick_Init(void);
void SysTick_Wait(unsigned long delay);
void SysTick_Wait10ms(unsigned long delay);