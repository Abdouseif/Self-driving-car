#include "tm4c123gh6pm.h"

void SysTick_Init(void){
  NVIC_ST_CTRL_R = 0;               // disable SysTick during setup
  NVIC_ST_CTRL_R = 0x00000005;      // enable SysTick with core clock
}
// The delay parameter is in units of the 80 MHz core clock. (12.5 ns)
void SysTick_Wait(unsigned long delay){
  NVIC_ST_RELOAD_R = 80*delay-1;  // number of counts to wait
  NVIC_ST_CURRENT_R = 0;       // any value written to CURRENT clears
  while((NVIC_ST_CTRL_R&0x00010000)==0){ // wait for count flag
  }
}
void SysTick_Wait10ms(unsigned long delay){
  unsigned long i;
  for(i=0; i<delay; i++){
    SysTick_Wait(800000);  // wait 10ms
  }
}


//Timer1 A with pins : PB0 trigger, PB4 Echo
void InitSensor_04(void)
{
	unsigned long delay;
	SYSCTL_RCGCGPIO_R |= 0x02;
	SYSCTL_RCGCTIMER_R |= 0x02;    				//Enable and provide a clock to 16/32-bit general-purpose timer module 0 in Run mode
	TIMER1_CTL_R &= ~0x01;								//Timer A is disabled
		
	TIMER1_CFG_R	 = 0x00000004;					//For a 16/32-bit timer, this value selects the 16-bit timer configuration
  TIMER1_TAMR_R 	|= 0x17;     					//Capture mode , Edge-Time mode , Capture or compare mode is enabled,  The timer counts up. When counting up, the timer starts from a
																				//value of 0x00	
	TIMER1_CTL_R |= 0x0C;									//Timer is sensetive to both edges
	TIMER1_TAILR_R = 0x0000FFFF;	
	TIMER1_CTL_R |= 0x01;									//Enable timerA
	
	
		
  SYSCTL_RCGC2_R |= 0x02;      					// 1) provide clock 
  delay = SYSCTL_RCGC2_R;      					// 2) no need to unlock
  GPIO_PORTB_AMSEL_R &= ~0x11; 					// 3) disable analog function on PB4-PB0
	GPIO_PORTB_PCTL_R |= 0x00070000; 			// 4) enable timer alternate function for PB4
  GPIO_PORTB_DIR_R |= 0x11;    					// 5) outputs on PB4-0
  GPIO_PORTB_AFSEL_R |= 0x10; 					// 6)Enable alternate function for PB4
	GPIO_PORTB_DEN_R |= 0x11;    					// 7) enable digital on PB5-0
}


//Timer1 B with pins : PB1 trigger, PB5 Echo
void InitSensor_15(void)
{
	unsigned long delay;
	SYSCTL_RCGCGPIO_R |= 0x02;
	SYSCTL_RCGCTIMER_R |= 0x02;    				//Enable and provide a clock to 16/32-bit general-purpose timer module 0 in Run mode
	TIMER1_CTL_R &= ~0x100;								//Timer B is disabled
		
	TIMER1_CFG_R	 = 0x00000004;					//For a 16/32-bit timer, this value selects the 16-bit timer configuration
  TIMER1_TBMR_R 	|= 0x17;     					//Capture mode , Edge-Time mode , Capture or compare mode is enabled,  The timer counts up. When counting up, the timer starts from a
																				//value of 0x00	
	TIMER1_CTL_R |= 0xC00;								//Timer is sensetive to both edges
	TIMER1_TBILR_R = 0x0000FFFF;	
	TIMER1_CTL_R |= 0x100;								//Enable timerB
	
	
		
  SYSCTL_RCGC2_R |= 0x02;      					// 1) provide clock 
  delay = SYSCTL_RCGC2_R;      					// 2) no need to unlock
  GPIO_PORTB_AMSEL_R &= ~0x22; 					// 3) disable analog function on PB5-PB1
	GPIO_PORTB_PCTL_R |= 0x00700000; 			// 4) enable timer alternate function for PB5
  GPIO_PORTB_DIR_R |= 0x22;    					// 5) outputs on PB5-1
  GPIO_PORTB_AFSEL_R |= 0x20; 					// 6)Enable alternate function for PB5
	GPIO_PORTB_DEN_R |= 0x22;    					// 7) enable digital on PB5-1
}



double MeasureDistance_04(void)
{
	unsigned long RisingEdge;
	unsigned long FallingEdge;
	 double time;
	unsigned long counter;
	
	TIMER1_TAV_R = 0x0000;	 
  
		
		GPIO_PORTB_DATA_R &= ~0x01;
		SysTick_Wait(2);
    GPIO_PORTB_DATA_R |= 0x01;
		SysTick_Wait(12);
		GPIO_PORTB_DATA_R &= ~0x01;
		 
    
		 
		TIMER1_ICR_R |= 0x04;
    while((TIMER1_RIS_R & 0x00000004)==0){}							
	//Wait till captured
	
	  RisingEdge = 	TIMER1_TAR_R; 		//Time of rising edge
		TIMER1_ICR_R |=0x04; 														//clear timer capture flag
		while((TIMER1_RIS_R & 0x00000004)==0){}							//Wait till captured
	
			
		FallingEdge = TIMER1_TAR_R; 		//Time of falling edge
			
		time = (double)(FallingEdge - RisingEdge);
		time = time * 12.5 /(1000000000);
		time = (time*175000)/2;
		
		return time;
}




double MeasureDistance_15(void)
{
	unsigned long RisingEdge;
	unsigned long FallingEdge;
	 double time;
	unsigned long counter;
	
	TIMER1_TBV_R = 0x0000;	 
  
		
		GPIO_PORTB_DATA_R &= ~0x02;
		SysTick_Wait(2);
    GPIO_PORTB_DATA_R |= 0x02;
		SysTick_Wait(12);
		GPIO_PORTB_DATA_R &= ~0x02;
		 
    
		 
		TIMER1_ICR_R |= 0x400;
    while((TIMER1_RIS_R & 0x00000400)==0){}							
	//Wait till captured
	
	  RisingEdge = 	TIMER1_TBR_R; 		//Time of rising edge
		TIMER1_ICR_R |=0x400; 														//clear timer capture flag
		while((TIMER1_RIS_R & 0x00000400)==0){}							//Wait till captured
	
			
		FallingEdge = TIMER1_TBR_R; 		//Time of falling edge
			
		time = (double)(FallingEdge - RisingEdge);
		time = time * 12.5 /(1000000000);
		time = (time*175000)/2;
		//SysTick_Wait(60000);
		return time;
}


//Timer0 A with pins : PB2 trigger, PB6 Echo
void InitSensor_26(void)
{
	
	unsigned long delay;	
	SYSCTL_RCGCGPIO_R |= 0x02;
	SYSCTL_RCGCTIMER_R |= 0x01;      				//Enable and provide a clock to 16/32-bit general-purpose timer module 0 in Run mode
	TIMER0_CTL_R &= ~0x01;								//Timer A is disabled
		
	TIMER0_CFG_R	 = 0x00000004;					//For a 16/32-bit timer, this value selects the 16-bit timer configuration
  TIMER0_TAMR_R 	|= 0x17;     					//Capture mode , Edge-Time mode , Capture or compare mode is enabled,  The timer counts up. When counting up, the timer starts from a
																				//value of 0x00	
	TIMER0_CTL_R |= 0x0C;									//Timer is sensetive to both edges
	TIMER0_TAILR_R = 0x0000FFFF;	
	TIMER0_CTL_R |= 0x01;									//Enable timerA
	
	
	
  SYSCTL_RCGC2_R |= 0x02;      					// 1) provide clock 
  delay = SYSCTL_RCGC2_R;    
  GPIO_PORTB_LOCK_R= 0x4C4F434B ;                         	// 2) no need to unlock
  GPIO_PORTB_CR_R= 0x0C;                    	// 2) no need to unlock
  GPIO_PORTB_AMSEL_R &= ~0x44; 					// 3) disable analog function on PB4-PB0
	GPIO_PORTB_PCTL_R |= 0x07000000; 			// 4) enable timer alternate function for PB4
  GPIO_PORTB_DIR_R |= 0x44;    					// 5) outputs on PB4-0
  GPIO_PORTB_AFSEL_R |= 0x40; 					// 6)Enable alternate function for PB4
	GPIO_PORTB_DEN_R |= 0x44;    					// 7) enable digital on PB5-0
}


double MeasureDistance_26(void)
{
	unsigned long RisingEdge;
	unsigned long FallingEdge;
  double time;
	unsigned long counter;
	
	TIMER0_TAV_R = 0x0000;	 
  
		
		GPIO_PORTB_DATA_R &= ~0x04;
		SysTick_Wait(2);
    GPIO_PORTB_DATA_R |= 0x04;
		SysTick_Wait(12);
		GPIO_PORTB_DATA_R &= ~0x04;
		 
    
		 
		TIMER0_ICR_R |= 0x04;
    while((TIMER0_RIS_R & 0x00000004)==0){}							
	//Wait till captured
	
	  RisingEdge = 	TIMER0_TAR_R; 		//Time of rising edge
		TIMER0_ICR_R |=0x04; 														//clear timer capture flag
		while((TIMER0_RIS_R & 0x00000004)==0){}							//Wait till captured
	
			
		FallingEdge = TIMER0_TAR_R; 		//Time of falling edge
			
		time = (double)(FallingEdge - RisingEdge);
		time = time * 12.5 /(1000000000);
		time = (time*175000)/2;
		
		return time;
}