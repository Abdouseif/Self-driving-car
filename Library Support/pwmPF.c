#include "pwmPF.h"

void Init_PWM_PF2(unsigned int duty)
{ volatile unsigned long delay;
	float Pduty;
	
	PortF_Init();
	Pduty = (((100.0-duty)/100.0)*0x190)-0x01;
	SYSCTL_PRPWM_R  |= 0x2;
	delay = SYSCTL_PRPWM_R;
	SYSCTL_RCGCPWM_R = 0x2;
	delay = SYSCTL_RCGCPWM_R; 
	PWM1_CTL_R  = 0x00000008;
	PWM1_3_CTL_R  = 0x00000000;
	delay = PWM1_3_CTL_R;
	PWM1_3_GENA_R = 0x0000008C;
	PWM1_3_LOAD_R = 0x0000018F;
	PWM1_3_CMPA_R = Pduty;
	PWM1_3_CTL_R = 0x01;
	delay = PWM1_3_CTL_R;
	PWM1_ENABLE_R |= 0x40;
}

void Set_PWM_PF2(unsigned int duty)
{ float Pduty;
	Pduty = (((100.0-duty)/100.0)*0x190)-0x01;
	PWM1_3_CMPA_R = Pduty;
}

void Init_PWM_PF3(unsigned int duty)
{ volatile unsigned long delay;
	float Pduty;
	
	PortF_Init();
	Pduty = (((100.0-duty)/100.0)*0x190)-0x01;
	SYSCTL_PRPWM_R  |= 0x2;
	delay = SYSCTL_PRPWM_R;
	SYSCTL_RCGCPWM_R = 0x2;
	delay = SYSCTL_RCGCPWM_R;
	PWM1_CTL_R  = 0x00000008;
	PWM1_3_CTL_R  = 0x00000000;
	delay = PWM1_3_CTL_R;
	PWM1_3_GENB_R = 0x0000080C;
	PWM1_3_LOAD_R = 0x0000018F;
	PWM1_3_CMPB_R = Pduty;
	PWM1_3_CTL_R = 0x01;
	delay = PWM1_3_CTL_R;
	PWM1_ENABLE_R |= 0x80;
}

void Set_PWM_PF3(unsigned int duty)
{ float Pduty;
	Pduty = (((100.0-duty)/100.0)*0x190)-0x01;
	PWM1_3_CMPB_R = Pduty;
}

void PortF_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000020;      // 1) F clock
  delay = SYSCTL_RCGC2_R;            // delay to allow clock to stabilize   
	SYSCTL_RCGC0_R |= 0x00100000; 
	delay =	SYSCTL_RCGC0_R;
  GPIO_PORTF_AMSEL_R &= 0x00;        // 2) disable analog function
	GPIO_PORTF_AFSEL_R |= 0x0C;
  GPIO_PORTF_PCTL_R |= 0x00005500;   // 3) GPIO PWM bit PCTL  
  GPIO_PORTF_DIR_R |= 0x0E;          // 4.2) PF3,2,1 output         
  GPIO_PORTF_DEN_R |= 0x1E;          // 7) enable digital pins PF4-PF1
	
}

void Delay1ms(unsigned long time){
  unsigned long i;
  while(time > 0){
    i = 13333;  // this number means 1ms
    while(i > 0){
      i = i - 1;
    }
    time = time - 1; // decrements every 100 ms
  }
}