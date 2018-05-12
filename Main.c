#include "Library Support/tm4c123gh6pm.h"
#include "Library Support/linesensors.h"
#include "Library Support/MotorDriver.h"
#include "Library Support/SensorInterface.h"
#include <math.h>
#include <stdint.h>


int x;
enum{HEAD, CW, ACW};
enum {MF_ALL, MF_L, MF_R, MB_ALL, MB_L, MB_R, BRAKE , MFB, MBF,B, TARGETING, LSFR, LSFL, LSFRL, LSBR, LSBL, LSBRL, NA};
enum {NORMAL, AF1, AF2, AF3};
/////////////////////////////////////////////////////////
typedef struct state {
	
	int Heading;
	int Action;
	int SigmoidOperand;
	int isLineFound;
	int Mode;
	int Pervstate;
} state;
//////////////////////////////////////////////////////////

state CurrentState;
int L_Current_Speed;
int R_Current_Speed;
void RunAction(state*);
void DetectState(state*PerviousState);
void EnableInterrupts(void);
void Reset_Handler(void);
////////////////////////////////////////////////////////

void SystemInit()
{}
 
////////////////////////////////////////////////////////
void GPIOPortA_Handler(void)
	{
		unsigned short Sense;
		GPIO_PORTA_ICR_R |= 0x3C;
		Sense = Which_Sensor();
			if (Sense == FR)
			{
				while (Sense == FR)
				{
				Sense = Which_Sensor();
					R_Current_Speed= 75;
					L_Current_Speed= 75;
					FWD_BWD_Move(R_Current_Speed, L_Current_Speed);
				   
				}
					
			}
			else if (Sense == FL)
			{
				while (Sense == FL)
				{
				Sense = Which_Sensor();
				R_Current_Speed= 75;
				L_Current_Speed= 75;
				BWD_FWD_Move(R_Current_Speed, L_Current_Speed);
				}
			}
		 else if (Sense == FRL)
			{
				while (Sense == FRL)
				{
				Sense = Which_Sensor();
				R_Current_Speed= 75;
				L_Current_Speed= 75;
				BWD_FWD_Move(R_Current_Speed, L_Current_Speed);
				}
			}
		
		return;
	}
	
	////////////////////////////////////////////////////////

int main(void)
{
	
	EnableInterrupts();
	Init_Motors();    //Initialize PWM for Motors on PF2 and PF3. Control Pins: PC4, PC5, PC6, PC7.
	SysTick_Init();
	InitSensor_04();  //Initialize Left Distance Sensor on PB0 and PB4.
	InitSensor_15();  //Initialize Middle Distance Sensor on PB1 and PB5.
	InitSensor_26();	//Initialize Right Distance Sensor on PB2 and PB6.
	CurrentState.Mode=NORMAL; //initialization of the current state
	CurrentState.Action=MF_ALL;
	CurrentState.Pervstate=1;
  SensorsInit();    //Line Found interrupt Sensors on PA2, PA3, PA4, PA5

	
	while(1)
	{
		DetectState(&CurrentState);
		RunAction(&CurrentState);
	}
}

//////////////////////////////////////////***********************//////////////////////////////////////
void DetectState(state*PerviousState)
{
	state NextState;
	double LSense; 
	double MSense;
	double RSense;
	double S; 
	double SRL;
	int i;
	
	LSense = MeasureDistance_04();  
	MSense	= MeasureDistance_15();
	RSense = MeasureDistance_26();

	S=20; //the distance of Middle sensor to take action
	SRL=10; //the distance of R&L senszors to take action
	
	//////////////////////////////////////////////////////
	if (PerviousState->Mode == NORMAL) //Normal Mode No Alternative Functions
	{
		if((LSense>SRL) && (MSense>S) && (RSense>SRL)) // forward Move and all sensors Low (1)
		{ 
			  NextState.Action = MF_ALL;
				NextState.Mode = NORMAL;
		}
		
		else if((LSense>SRL) && (MSense>S) && (RSense<=SRL)) // Turn left on speed  (2)
			{		
				if(PerviousState->Action==MBF)
				{
					NextState = *PerviousState;
					NextState.Mode = AF2; 
				}
				else
					  {
   			NextState.Action = MFB;
				NextState.Mode = NORMAL;
				    }
			}
		
		else if((LSense<=SRL) && (MSense>S) && (RSense>SRL)) // turn right on speed (4)
			{
				if(PerviousState->Action==MFB)
				{
					NextState = *PerviousState;
					NextState.Mode = AF2;
				}
				else
				{
			  NextState.Action = MBF;
				NextState.Mode = NORMAL;
				}
			}
		
			else if((LSense>SRL) && (MSense<=S) && (RSense>SRL)) //turn right or left (6)
			{
				if(LSense>RSense && !(PerviousState->Action==MBF))
				{  
	       NextState.Action = MFB;
				}
        else	
				{ 
			  NextState.Action = MBF;	
				}
				NextState.Mode = NORMAL;

			}
			else if((LSense>SRL) && (MSense<=S) && (RSense<=SRL)) //(7)
			{
				if(PerviousState->Action==MBF)
				{
					NextState = *PerviousState;
					NextState.Mode = AF2;
				}
				else
					{
			  NextState.Action = MFB;
				NextState.Mode = NORMAL;
				}
        }
			else if((LSense<=SRL) && (MSense<=S) && (RSense>SRL)) //(8)
			{
				if(PerviousState->Action==MFB)
				{
					NextState = *PerviousState;
					NextState.Mode = AF2;
				}
				else
					 {
			     NextState.Action = MBF;
				   NextState.Mode = NORMAL;
				   }
       }
			/////////***********************************//////////////////////////////////
			/////////***********************************/////////////////////////////////
			else if((LSense<=SRL) && (MSense>S) && (RSense<=SRL)&&(LSense<=10) && (RSense<=10))
			{ 
				NextState.Action = B;   // decrease speed before entering the altern function
				NextState.Mode = AF2;
			}
				else if  ((LSense<=SRL) && (MSense>S) && (RSense<=SRL)&&(LSense>10) && (RSense>10))// Altern. function Number 1
			{ 
				NextState.Action = MF_ALL; 
				NextState.Mode = NORMAL;
			}
			else if((LSense<=10)&& (MSense>S) && (RSense>10)&&(RSense<=SRL)) // Altern. function Number 1
			{
					if(PerviousState->Action==MFB)
				{
					NextState = *PerviousState;
					NextState.Mode = AF2;
				}
				else{
				NextState.Action = MBF;
				NextState.Mode = NORMAL;
				}
			}
			else if((LSense>10)&&(LSense<=SRL)&&(MSense>S)&&(RSense<=10)) // Altern. function Number 1
			{
					if(PerviousState->Action==MBF)
				{
					NextState = *PerviousState;
					NextState.Mode = AF2;
				}
				else
					  {
				NextState.Action = MFB;
				NextState.Mode = NORMAL;
				    }
			}
			/////////////////***************************///////////////////////////////
			///////////////////////////////////////////////////////////////////////////
			else if((LSense<=SRL) && (MSense<=S) && (RSense<=SRL)) //Altern function Number 2
			{
				NextState.Action = B; // stop brfore entering the altern function
				NextState.Mode = AF2;
			}
			///////////////////////////////////////////////////////////////////////////
		}
			else if (PerviousState->Mode == AF2)
			{
				NextState.Action = MBF;
				if(RSense>SRL&&LSense>SRL&&MSense>S)
				{NextState.Mode = NORMAL;}
				else{NextState.Mode = AF2;}
			 
	    }
			*PerviousState = NextState;

}
			

/////////////////////////////////////////////////****************************////////////////////////////////////////////////////
void RunAction(state *CurrentState)
{
	double dist, dist1, dist2, dist3;
	int x;
	switch(CurrentState->Action)
	{
		case MF_ALL:
		FWD_Move(59,59); 
		break;
		
		case MF_L:
			R_Current_Speed=100;
			L_Current_Speed = 10;
			FWD_Move(R_Current_Speed, L_Current_Speed);
		break;
		
		case MF_R:
			L_Current_Speed=100;
			R_Current_Speed = 10;
			FWD_Move(R_Current_Speed, L_Current_Speed);
		break;
		
		case BRAKE:
			Full_Brake();
		break;

		case MFB:
		FWD_Move(100, 20);
		R_Current_Speed= 63;
		L_Current_Speed= 63;
	  FWD_BWD_Move(R_Current_Speed, L_Current_Speed);
		break;

		case MBF:
			FWD_Move(20,100);
			L_Current_Speed= 63;
			R_Current_Speed= 63;
			BWD_FWD_Move(R_Current_Speed, L_Current_Speed);
		break;		

		case B:
				FWD_Move(20, 20);
		break;
	
	}	
}


