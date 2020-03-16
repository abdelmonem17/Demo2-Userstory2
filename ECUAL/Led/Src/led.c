
/****************************************************************************
 * @file led.h
 * @brief this file includes some functions prototype to leds
 *
 * @author Abdelmonem Mostafa
 * @date  OCT 2019
 *
 ****************************************************************************/

#include "led.h"

void Leds_Init(void)
{

	Gpio_PinDirection(LEDS_PORT,LEDS,OUTPUT);
	Gpio_PinWrite_Same_Value(LEDS_PORT,LEDS,0);
}

void Leds_Toggle(uint8 leds)
{
	
	Gpio_PinToggle(LEDS_PORT,leds);
}

void Leds_On(uint8 leds)
{
	
	Gpio_PinWrite_Same_Value(LEDS_PORT,leds,HIGH);
}

void Leds_Off(uint8 leds)
{
	
	Gpio_PinWrite_Same_Value(LEDS_PORT,leds,LOW);
}