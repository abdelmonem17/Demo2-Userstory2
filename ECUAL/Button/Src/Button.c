/*
 * Button.c
 *
 * Created: 12/2/2019 11:42:52 AM
 *  Author: AVE-LAP-062
 */ 

#include "Button.h"

static uint8 BTN_Read(uint8 btn);

/****************************************************************************
*
*   @brief function inits the button
*   this function takes void and return void
*   @params  name : void
*
*   @return void
****************************************************************************/
void BTN_init(void)
{
	BUTTON0_DIR &= ~(1<<BUTTON0_PIN);
	BUTTON1_DIR &= ~(1<<BUTTON1_PIN);

}
static uint8 BTN_Read(uint8 btn)
{
	static uint8 BTN0Checking_counts=NUMBER_ZERO;
	static uint8 BTN1Checking_counts=NUMBER_ZERO;
	uint8 button_status=LOW;
	switch(btn)
	{
		case BTN0:
			if( (Gpio_PinRead(BUTTON0_PORT,BUTTON0_PIN) == HIGH)  )
			{
				if ((BTN0Checking_counts == NUMBER_ONE))
				{
					button_status =HIGH;
					BTN0Checking_counts=NUMBER_ZERO;
				}
				else
				{
					BTN0Checking_counts++;
				}
				
			}
			else
			{
				button_status=NUMBER_ZERO;
				BTN0Checking_counts =NUMBER_ZERO;
				
			}
			break;
		case BTN1:
			if( (Gpio_PinRead(BUTTON1_PORT,BUTTON1_PIN) == HIGH)  )
			{
				if ((BTN1Checking_counts == NUMBER_ONE))
				{
					button_status =HIGH;
					BTN1Checking_counts=NUMBER_ZERO;
				}
				else
				{
					BTN1Checking_counts++;
				}
				
			}
			else
			{
				button_status=NUMBER_ZERO;
				BTN1Checking_counts =NUMBER_ZERO;
				
			}
			break;
	}
	return button_status;
}
uint8 BTN_Read_Undebounce(uint8 btn)
{
	BTN_Read(btn);
	return BTN_Read(btn);
}