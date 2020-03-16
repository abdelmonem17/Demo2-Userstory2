

#ifndef USER_STROY_2_H_
#define USER_STROY_2_H_





#include "types.h"

#define REQ2_TASKS_COUNTS_													5
#define REQ2_TASKS_ERROR_REPORT_BUFFER_NULL									(uint8)-1
#define REQ2_TASKS_SUCCFULLY_CREATED										(uint8) 1
#define REQ2_TASKS_NOTSUCCFULLY_CREATED										(uint8)	0
#define REQ2_TASKS_NOTSUCCFULLY_CREATED_LCD_MUTUX_FAILED					(uint8)	-2

#define READ_BIT_BYTE(byte,bit)	  ((bit & byte) !=0) 


/****************************************************************************
*
*   @brief function acts as task which monitors LED0
*   this function takes void and return void to turn led on and off
*   @params  name : void
*
*   @return void
****************************************************************************/

void req2_Led_Task( void * pvParameters);



/****************************************************************************
*
*   @brief function acts as task which monitors button 0
*   this function takes pvParameters and return void
*   @params  name : void
*
*   @return void
****************************************************************************/
void req2_Button0_Task( void * pvParameters);



/****************************************************************************
*
*   @brief function acts as task which monitors button 1
*   this function takes pvParameters and return void
*   @params  name : void
*
*   @return void
****************************************************************************/
void req2_Button1_Task( void * pvParameters);

/****************************************************************************
*
*   @brief function acts as task which monitors buttons and print on the LCD
*   this function takes pvParameters and return void
*   @params  name : void
*
*   @return void
****************************************************************************/
void req2_Lcd_Task( void * pvParameters);


/****************************************************************************
*
*   @brief function acts as task which inits the system
*   this function takes pvParameters and return void
*   @params  name : void
*
*   @return void
****************************************************************************/
void req2_Init_Task(void * pvParameters);


/****************************************************************************
*
*   @brief function creates tasks
*   this function takes errorBuffer to fill with the status of the tasks creation and returns status of the creation
*   @params  name : errorBuffer
*	type :uint8 *
*   @return uint8
****************************************************************************/
uint8 req2_tasksCreation(uint8 *errorBuffer);




#endif /* USER_STROY 2_H_ */