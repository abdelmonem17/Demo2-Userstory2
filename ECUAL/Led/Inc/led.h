/*
 * led.h
 *
 * Created: 10/22/2019 10:36:04 AM
 *  Author: AVE-LAP-062
 */ 


#ifndef LED_H_
#define LED_H_
#include "ledConfig.h"




void Leds_Init(void);
void Leds_Toggle(uint8 leds);
void Leds_On(uint8 leds);
void Leds_Off(uint8 leds);


#endif /* LED_H_ */