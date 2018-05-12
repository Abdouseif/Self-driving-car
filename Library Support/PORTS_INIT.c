#include "PORTS_INIT.h"


void PortC_Init(void)
{
	volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x4;      // 1) C clock
  delay = SYSCTL_RCGC2_R;            // delay to allow clock to stabilize   
  GPIO_PORTC_AMSEL_R = 0x00;        // 2) disable analog function
	GPIO_PORTC_AFSEL_R = 0x00;
  GPIO_PORTC_PCTL_R |= 0x00000000;   // 3) GPIO PWM bit PCTL  
  GPIO_PORTC_DIR_R |= 0xF0;          // 4.2)         
  GPIO_PORTC_DEN_R |= 0xFF;          // 7) enable digital pins PF4-PF1
	
}

void PortE_Init(void)
{
	
		volatile unsigned long delay;
  SYSCTL_RCGC2_R |= (1<<4);      // 1) E clock
  delay = SYSCTL_RCGC2_R;            // delay to allow clock to stabilize   
  GPIO_PORTE_AMSEL_R = 0x00;        // 2) disable analog function
	GPIO_PORTE_AFSEL_R = 0x00;
  GPIO_PORTE_PCTL_R |= 0x00000000;   // 3) GPIO  
  GPIO_PORTE_DIR_R |= 0x00;          // 4.2)         
  GPIO_PORTE_DEN_R |= 0xFF;          // 7) enable digital pins PE
	
}