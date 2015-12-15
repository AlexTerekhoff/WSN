#include "../include/EVB8871.h"

#define DEFAULT_PRESSING_TIME 10
#define DEFAULT_INTERVAL_TIME 5
#define DEFAULT_REDLIGHT_TIME 30
#define DEFAULT_BLUELIGHT_TIME 30
#define DEFAULT_YELLOWLIGHT_TIME 10

volatile unsigned int timeCounter;             		
volatile unsigned int timerInterruptCounter;

volatile unsigned int blueButtonState = 0; 
volatile unsigned int blackButtonState = 0;    
volatile unsigned int blueButtonWasPressed = 0;
volatile unsigned int blackButtonWasPressed = 0;            

volatile unsigned int lightsOn = 0; 
volatile unsigned int shouldBePresenentYellowLight = 0;
volatile unsigned int flag = 0;

void init();
void handleUserActions();
void handleFirstTapOnButton();
void handleSecondTapOnButton();
void turnOnLights();
int defineWichButtonWasPressed();
int defineWichButtonWasRealesed();

int main(void) 
{
	init();
	while(1)
	{
		handleUserActions();	
	}
	return 0;
}

void init()
{
//	Настроим сторожевой таймер
	WDTCTL = WDTPW + WDTHOLD;

	//	Подтянем кнопку sw1, sw2
	P2REN |= BIT2 | BIT3;
	P2IFG &= ~BIT2 | ~BIT3;
	P2IES |= BIT2 |BIT3;
	P2IE |= BIT2 | BIT3;
	P2OUT |= BIT2 | BIT3;

	//	Настроим порты светодиодов
	P7DIR |= LED_RED|LED_YELLOW|LED_GREEN|LED_BLUE;

	//	Погасим светодиоды
	LEDS_PORT = 0xf8;
	//	Выберем XT2 в качестве MCLK
	SetMCLK(MCLK_XT2);
	//	Настроим таймер TA0
	TA0CCTL0 = CCIE;
	TA0CCR0 = 6000;
	//	Источнник: SMCLK, Режим: UP/down, Очистим TAR, Делитель: 8
	TA0CTL = TASSEL_2 + MC_3 + TACLR + ID_3;
	//	Обнулим счетчик            
	timerInterruptCounter = 0;
	timeCounter = 0;
//	Разрешим прерывания
	__enable_interrupt();
	//	Для отладки
	__no_operation();
}

void handleUserActions()
{
	if (lightsOn)
	{
		turnOnLights();
	}
	else
	{
		if(!flag)
		{
			handleFirstTapOnButton();
		}
		else
		{
			handleSecondTapOnButton();
		} 
	}	
}

int defineWichButtonWasPressed()
{
	int idButton = 0;
	if (blueButtonState && !blueButtonWasPressed)
	{
		blueButtonWasPressed = 1;
		idButton = 1;
	}
	else if (blackButtonState && !blackButtonWasPressed)
	{
		blackButtonWasPressed = 1;
		idButton = 2;
	}

	return idButton;
}

int defineWichButtonWasRealesed()
{	
	int idButton = 0;
	if (!blueButtonState && blueButtonWasPressed)
	{
		blueButtonWasPressed = 0;
		idButton = 1;
	}
	else if (!blackButtonState && blackButtonWasPressed)
	{
		blackButtonWasPressed = 0;
		idButton = 2;
	}

	return idButton;
}

void handleFirstTapOnButton()
{
	if(defineWichButtonWasPressed() > 0) timeCounter = 0;
	int idButton = defineWichButtonWasRealesed();
	if (idButton > 0)
	{
		if (timeCounter <= DEFAULT_PRESSING_TIME) flag = idButton;
		timeCounter = 0;
    		blueButtonWasPressed = 0;	
		blackButtonWasPressed = 0;
	}
}

void handleSecondTapOnButton()
{
	if(defineWichButtonWasPressed() > 0)
	{
		if (timeCounter > DEFAULT_INTERVAL_TIME) shouldBePresenentYellowLight = 1;
		timeCounter = 0;
	}
	int idButton = defineWichButtonWasRealesed();
	if (idButton == flag)
	{
		if (timeCounter <= DEFAULT_PRESSING_TIME)
    		{
    			lightsOn = 1;
			flag = idButton;
    		}
    		timeCounter = 0;
	}	
}
	
void turnOnLights()
{
	if (shouldBePresenentYellowLight)
	{
		SetLed(LED_YELLOW, 1);
		if (timeCounter >  DEFAULT_YELLOWLIGHT_TIME )
		{
			SetLed(LED_YELLOW, 0);
			shouldBePresenentYellowLight = 0;
			lightsOn = 0;
			flag = 0;
		}	
	}
	else if(flag == 2)
	{
		SetLed(LED_RED, 1);
		if (timeCounter > DEFAULT_BLUELIGHT_TIME)
		{
			SetLed(LED_RED, 0);
			lightsOn = 0;
			flag = 0;
		}
	}
	else if(flag == 1)
	{
		SetLed(LED_GREEN, 1);
		if (timeCounter > DEFAULT_REDLIGHT_TIME)
		{
			SetLed(LED_GREEN, 0);
			lightsOn = 0;
			flag = 0;
		}
	}
}
    
//	Обработчик прерываний для TA0
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void) 
{
	++timerInterruptCounter;
	if (timerInterruptCounter == 25)
	{
		timerInterruptCounter = 0;
		++timeCounter;
	}
}

#pragma vector = PORT2_VECTOR
__interrupt void P2_ISR(void)
{
	switch(P2IFG) 
	{
		case BIT3:
		P2IFG &= ~BIT3;
		blueButtonState = blueButtonState ? 0 : 1;
		if(blueButtonState)
		{
			P2IES &= ~BIT3;

		}
		else
		{
			P2IES |= BIT3;
		}
		return;

		case BIT2:
		P2IFG &= ~BIT2;
		blackButtonState = blackButtonState ? 0 : 1;
		if(blackButtonState)
		{
			P2IES &= ~BIT2;

		}
		else
		{
			P2IES |= BIT2;
		}
		
		return;
	}
}

