#include "CompassInterface.h"


void CompassInit(void)
{
		PortE_Init();
}
int ReadCompass(void)
{
	if(GPIO_PORTE_DATA_R & 0x04)
	{
		return 0;
	}
	else if(GPIO_PORTE_DATA_R & 0x08)
	{
		return 1;
	}
	else if(GPIO_PORTE_DATA_R & 0x10)
	{
		return 2;
	}
}
	