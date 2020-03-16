/*
 * ledConfig.h
 *
 * Created: 10/22/2019 10:29:29 AM
 *  Author: AVE-LAP-062
 */ 


#ifndef LEDCONFIG_H_
#define LEDCONFIG_H_
#include "gpio.h"

#define LEDS_PORT		PORT_B



#define LED0	0x10
#define LED1	0x20
#define LED2	0x40
#define LED3	0x80

#define  LEDS (LED0|LED1|LED2|LED3)



#endif /* LEDCONFIG_H_ */