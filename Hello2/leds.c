//	leds.c - управление светодиоидами

#include "../include/EVB8871.h"

//	Установим состояние светодиоида x
void SetLed(int led, int state) {
	LEDS_PORT = (LEDS_PORT & ~led) | (state ? 0 : led);
}
