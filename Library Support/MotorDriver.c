#include "MotorDriver.h"
#include "PORTS_INIT.h"

void Init_Motors(void)
{
	PortC_Init();
	Init_PWM_PF2(0);
	Init_PWM_PF3(0);
}

void FWD_Move(unsigned int R, unsigned int L)
{
	GPIO_PORTC_DATA_R = 0x00000050;
	Set_PWM_PF2(R);
	Set_PWM_PF3(L);
}
void BWD_Move(unsigned int R, unsigned int L)
{
	GPIO_PORTC_DATA_R = 0x000000A0;
	Set_PWM_PF2(R);
	Set_PWM_PF3(L);
}
void FWD_BWD_Move(unsigned int R, unsigned int L)
{
	GPIO_PORTC_DATA_R = 0x00000090;
	Set_PWM_PF2(R);
	Set_PWM_PF3(L);
}

void BWD_FWD_Move(unsigned int R, unsigned int L)
{
	GPIO_PORTC_DATA_R = 0x00000060;
	Set_PWM_PF2(R);
	Set_PWM_PF3(L);
}


void Full_Brake(void)
{
	GPIO_PORTC_DATA_R = 0x000000F0;
}
void R_Half_Brake(void)
{
	GPIO_PORTC_DATA_R = 0x00000030;
}

void L_Half_Brake(void)
{
	GPIO_PORTC_DATA_R = 0x000000C0;
}