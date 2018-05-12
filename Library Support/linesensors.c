#include "linesensors.h"
#include "tm4c123gh6pm.h"
void PortA_Init()
{
volatile unsigned long delay;
SYSCTL_RCGCGPIO_R |= 0x01;
delay = SYSCTL_RCGC2_R;            // delay to allow clock to stabilize   
GPIO_PORTA_DIR_R |=0x00;
GPIO_PORTA_AMSEL_R|=0x00;
GPIO_PORTA_AFSEL_R |=0x00;
GPIO_PORTA_DEN_R |=0x3C;
//GPIO_PORTA_PDR_R |=0x3C;
GPIO_PORTA_IS_R |= 0x00;
GPIO_PORTA_IBE_R |= 0x00;
GPIO_PORTA_IEV_R |= 0x3C;
GPIO_PORTA_ICR_R |= 0x3C; // clear flags PA3-0
GPIO_PORTA_IM_R |= 0x3C; //arm interrupts on PA3-0
NVIC_PRI0_R |= 0x20; // priority 1 on portA interrupt.
NVIC_EN0_R = 0x00000001; 	
}
 
void SensorsInit(void)
{
	PortA_Init();
}

int Which_Sensor(void)
{
  if(GPIO_PORTA_DATA_R&0x04)
	{return FR;}
	else if (GPIO_PORTA_DATA_R&0x08)
	{return FL;}
	else if(GPIO_PORTA_DATA_R&0x10)
	{return BR;}
	else if (GPIO_PORTA_DATA_R&0x20)
	{return BL;}
	else if (GPIO_PORTA_DATA_R&0x0C)
	{return FRL;}
	else if (GPIO_PORTA_DATA_R&0x14)
	{return BRL;}
	else if (GPIO_PORTA_DATA_R&0x28)
	{return FRBR;}
	else if (GPIO_PORTA_DATA_R&0x30)
	{return FLBL;}	
	else {return NON;}
}
