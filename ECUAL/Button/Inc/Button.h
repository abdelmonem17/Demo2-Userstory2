/*
 * Button.h
 *
 * Created: 12/2/2019 11:40:07 AM
 *  Author: AVE-LAP-062
 */ 


#ifndef BUTTON_H_
#define BUTTON_H_
#include "types.h"
#include "gpio.h"

#define BUTTON0_PORT	PORT_C
#define BUTTON0_DIR		DIR_C
#define BUTTON0_IN		IN_C
#define BUTTON0_OUT		OUT_C
#define BUTTON0_PIN		4


#define BUTTON1_PORT	PORT_B

#define BUTTON1_DIR		DIR_B
#define BUTTON1_IN		IN_B
#define BUTTON1_OUT		OUT_B
#define BUTTON1_PIN		2
#define BTN0	0
#define BTN1	1

/****************************************************************************
*
*   @brief function inits the button
*   this function takes void and return void
*   @params  name : void
*
*   @return void
****************************************************************************/
void BTN_init(void);
uint8 BTN_Read_Undebounce(uint8 btn);

#endif /* BUTTON_H_ */