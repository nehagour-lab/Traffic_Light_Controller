// ***** 0. Documentation Section *****
// TableTrafficLight.c for Lab 10
// Runs on LM4F120/TM4C123
// Index implementation of a Moore finite state machine to operate a traffic light.
// east/west red light connected to PB5
// east/west yellow light connected to PB4
// east/west green light connected to PB3
// north/south facing red light connected to PB2
// north/south facing yellow light connected to PB1
// north/south facing green light connected to PB0
// pedestrian detector connected to PE2 (1=pedestrian present)
// north/south car detector connected to PE1 (1=car present)
// east/west car detector connected to PE0 (1=car present)
// "walk" light connected to PF3 (built-in green LED)
// "don't walk" light connected to PF1 (built-in red LED)

// ***** 1. Pre-processor Directives Section *****
#include "TExaS.h"
#include "tm4c123gh6pm.h"

// ***** 2. Global Declarations Section *****
struct state{
unsigned long out; // output signals or cars
unsigned long walk; // outputs signals for pedestrian
unsigned long time; // Delay 10ms
unsigned long Next[9]; //switches
};
typedef const struct state SType;

#define GoW 0 // go toward west
#define WW 1 // wait west
#define GoS 2 // go toward south
#define WS 3 // wait south
#define GoPed 4 // pedestrian "walk"
#define RUN1 5 // RUN1,RUN2,RUN3 hurry up toggling signal
#define RUN2 6
#define RUN3 7
#define stopPed 8 // pedestrains "do not walk"
//Inilalize FSM
SType FSM[8] ={

{0x21, 0x02, 50, {GoW, WW, GoW, WW, WW, WW, WW, WW}},
{0x22, 0x02, 35, {GoS, GoS, GoS, GoS, GoPed, GoS, GoPed, GoS}},
{0x0C, 0x02, 50, {GoS, GoS, WS, WS, WS, WS, WS, WS}},
{0x14, 0x02, 35, {GoW, GoW, GoW, GoW, GoPed, GoPed, GoW, GoPed}},
{0x24, 0x08, 50, {GoPed, RUN1, RUN1, RUN1, GoPed, RUN1, RUN1, RUN1}},
{0x24, 0x02, 15, {RUN2, RUN2, RUN2, RUN2, RUN2, RUN2, RUN2, RUN2}},
{0x24, 0x00, 15, {RUN3, RUN3, RUN3, RUN3, RUN3, RUN3, RUN3, RUN3}},
{0x24, 0x00, 15, {GoW, GoS, GoW, GoW, GoPed, GoS, GoW, GoW}}
};

unsigned long S; // current state
unsigned long input; // sensor inputs

// FUNCTION PROTOTYPES: Each subroutine defined
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void); // Enable interrupts

// initlize PLL

void Systick_Init(void); // initlize timer
void PortB_Init(void); // initlize port B
void PortE_Init(void); // initlize port E
void PortF_Init(void); // initlize port F
void SysTick_Wait(unsigned long delay);
void SysTick_wait10ms(unsigned long delay); // delay for 10 ms
void Init_All(void); // initlizer for all functions

// ***** 3. Subroutines Section *****
int main(void){
			TExaS_Init(SW_PIN_PE210, LED_PIN_PB543210,ScopeOff); // activate grader and set system clock to 80 MHz
			EnableInterrupts();
			Init_All(); // Activate all functions
			S = GoW;
				while(1){
				GPIO_PORTB_DATA_R = FSM[S].out;
				GPIO_PORTF_DATA_R = FSM[S].walk;
				SysTick_wait10ms(FSM[S].time);
				input = GPIO_PORTE_DATA_R;
				S = FSM[S].Next[input];
				}
	}

	
void Init_All(void) {
				Systick_Init();
				//Phase locked loop
				//PLL_Init();
				PortB_Init();
				PortE_Init();
				PortF_Init();
}

// PORT B INITIALIZATION
void PortB_Init(void){
				volatile unsigned long delay;
				SYSCTL_RCGC2_R |=0x02;
				delay = SYSCTL_RCGC2_R;
				GPIO_PORTB_LOCK_R |= 0x4C4F434B;
				GPIO_PORTB_CR_R |=0x3F;
				GPIO_PORTB_DIR_R |= 0x3F;
				GPIO_PORTB_AFSEL_R &= ~ (0x3F);
				GPIO_PORTB_DEN_R |= 0x3F;
				GPIO_PORTB_AMSEL_R &= ~ (0x3F);
				GPIO_PORTB_PCTL_R &= ~ (0x000FFFFF);
}
// PORT E INITIALIZATION
void PortE_Init(void){
				volatile unsigned long delay;
				SYSCTL_RCGC2_R |=0x10;
				delay = SYSCTL_RCGC2_R;
				GPIO_PORTE_LOCK_R |= 0x4C4F434B;
				GPIO_PORTE_CR_R |= 0x07;
				GPIO_PORTE_DIR_R &= ~(0x07);
				GPIO_PORTE_AFSEL_R &= ~ (0x07);
				GPIO_PORTE_DEN_R |= 0x07;
				GPIO_PORTE_AMSEL_R &= ~ (0x07);
				GPIO_PORTE_PCTL_R &= ~ (0x00000FFF);
}
// PORT F INITIALIZATION
void PortF_Init(void){
				volatile unsigned long delay;
				SYSCTL_RCGC2_R |= 0x00000020;
				delay = SYSCTL_RCGC2_R;
				GPIO_PORTF_LOCK_R |= 0x4C4F434B;
				GPIO_PORTF_CR_R |= 0x0A;
				GPIO_PORTF_DIR_R |= (0x0A);
				GPIO_PORTF_AFSEL_R &= ~ (0x0A);
				GPIO_PORTF_DEN_R |= 0x0A;
				GPIO_PORTF_AMSEL_R &= ~ (0x0A);
				GPIO_PORTF_PCTL_R &= ~ (0x0000F0F0);
}
// SYSTICK INITIALIZATION
void Systick_Init(void){
				NVIC_ST_CTRL_R = 0;
				NVIC_ST_CTRL_R = 0x00000005;
}

// DELAY 10ms function
void SysTick_Wait (unsigned long delay){
			NVIC_ST_RELOAD_R = delay-1 ;
			NVIC_ST_CURRENT_R = 0;
			while ((NVIC_ST_CTRL_R&0x00010000) == 0){
			}
}

// Delay function
void SysTick_wait10ms(unsigned long delay){
			unsigned long i;
			for(i =0; i<delay; i++){
			SysTick_Wait(800000); // 10ms delay function 800000*12.5ns=10ms
			}
}
