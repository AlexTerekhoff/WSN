//	mclk.c - установка источника тактирования

#include "../include/EVB8871.h"
#include "../hal/HAL_PMM.h"
#include "../hal/HAL_UCS.h"

//	Установка источника тактирования
void SetMCLK(int Mode) {
	if(Mode == MCLK_XT2) {
		//	Зададим напряжения для ядра
		SetVCore(PMMCOREV_3);
		
		//	В качестве источника выберем XT2
		P5SEL |= 0x0C;
		
		SetLed(LED_RED,1);
		XT2_Start(XT2DRIVE_3);
		SetLed(LED_RED,0);
		
		//	Decrease XT2 Drive according to
		UCSCTL6 &= ~XT2DRIVE0;
		
		//	Для тактового сигнала MCLK - источник XT2
		SELECT_MCLK(SELS__XT2CLK);
		
		//	Для тактового сигнала SMCLK - источник XT2
		SELECT_SMCLK(SELS__XT2CLK);
	}
}
