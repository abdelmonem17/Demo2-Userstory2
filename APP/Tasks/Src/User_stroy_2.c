

/****************************************************************************
 * @file Req1_Led.h
 * @brief this file includes some functions implementation to manipulate leds using RTOS
 *
 * @author Abdelmonem Mostafa
 * @date  NOV 2019
 *
 ****************************************************************************/


#include "User_stroy_2.h"
#include "event_groups.h"
#include "task.h"
#include "led.h"
#include "lcd.h"
#include "Button.h"





/************************************************************************/
/*				macros to define tasks info                              */
/************************************************************************/
#define	 REQ2_LED_INIT_TASK_STACK_DEPTH									85
#define  REQ2_LED_TOGGLE_TASK_STACK_DEPTH								85
#define	 REQ2_BUTTON0_TASK_STACK_DEPTH									85
#define  REQ2_BUTTON1_TASK_STACK_DEPTH									85
#define  REQ2_LCD_TASK_STACK_DEPTH										85

#define	 REQ2_INIT_TASK_PRIORTIY										5
#define  REQ2_LED_TOGGLE_TASK_PRIORTY									3
#define  REQ2_BUTTON0_TASK_PRIORTY										2
#define  REQ2_BUTTON1_TASK_PRIORTY										1
#define  REQ2_LCD_TASK_PRIORTY											4

#define	 REQ2_INPUT_PARAM_REF											NULL




#define LCD_START_COLUMN_LED_STATUS										1
#define LCD_START_ROW_LED_STATUS										1
#define LCD_START_COLUMN_BUTTON_STATUS									1
#define LCD_START_ROW_BUTTON_STATUS										2

#define LCD_DISPLAYING_AREA												16



#define INIT_TASK_PERIODICITY											15
#define BUTTON0_TASK_PERIODICITY										20
#define BUTTON1_TASK_PERIODICITY										25
#define LCD_TASK_PERIODICITY											1
#define LED_TASK_PERIODICITY											30



#define LCD_DISPLAY_STATE_WAIT_LCD_EVENT_BIT							(uint8)0
#define LCD_DISPLAY_STATE_CLEAR_AREA									(uint8)1
#define LCD_DISPLAY_STATE_GOTO_ROW_LOCATION_LED_STATUS					(uint8)2
#define LCD_DISPLAY_STATE_DISPLAY_STRING_LED_STATUS						(uint8)3

#define LCD_DISPLAY_STATE_GOTO_ROW_LOCATION_BUTTON_STATUS				(uint8)4
#define LCD_DISPLAY_STATE_DISPLAY_STRING_BUTTON_STATUS					(uint8)5

#define LCD_DISPLAY_STATE_LAST_STRING_DELAY								(uint8)4

#define BUTTON0_STATUS_FLAG												BIT0
#define BUTTON1_STATUS_FLAG												BIT1

#define BUTTONS_CHANGING_STATUS_LED_FLAG								BIT3
#define BUTTONS_CHANFING_STATUS_LCD_FLAG								BIT4


/************************************************************************/
/*                         global static for tasks events                */
/************************************************************************/
static TaskHandle_t	req2_LedToggle_TaskHandler;
static TaskHandle_t	req2_Lcd_TaskHandler;
static TaskHandle_t	req2_Init_TaskHandler;
static TaskHandle_t	req2_Button0_TaskHandler;
static TaskHandle_t	req2_Button1_TaskHandler;
static  EventGroupHandle_t gEventGroupHandle_Buttons;



/****************************************************************************
*
*   @brief function acts as task which monitors LED0
*   this function takes void and return void to turn led on and off
*   @params  name : void
*
*   @return void
****************************************************************************/

void req2_Led_Task( void * pvParameters)
{
	EventBits_t au8_LedMonitorStatusFlag;
	EventBits_t au8_buttonsStatusFlags;
	while(1)
	{
		
		/************************************************************************/
		/*			wait for until any change in buttons                                                                   */
		/************************************************************************/
		au8_LedMonitorStatusFlag= xEventGroupWaitBits(
		gEventGroupHandle_Buttons,   /* The event group being tested. */
		BUTTONS_CHANGING_STATUS_LED_FLAG, /* The bits within the event group to wait for. */
		pdTRUE,        /* BIT_0 & BIT_4 should be cleared before returning. */
		pdFALSE,       /* Don't wait for both bits, either bit will do. */
		UINT16_MAX );/* Wait a maximum of 100ms for either bit to be set. */
		
		
		/* detect which button pressed */
		au8_buttonsStatusFlags = xEventGroupGetBits(gEventGroupHandle_Buttons );
		
		/************************************************************************/
		/*			check for until if change in buttons                                                                   */
		/************************************************************************/
		if ( (au8_LedMonitorStatusFlag & BUTTONS_CHANGING_STATUS_LED_FLAG) != FALSE)
		{
			if ( ((au8_buttonsStatusFlags & BUTTON0_STATUS_FLAG) !=FALSE) || ( (au8_buttonsStatusFlags & BUTTON1_STATUS_FLAG) !=FALSE) )
			{
				Leds_On(LED0);
			}
			else
			{
				Leds_Off(LED0);
			}
			
			
		}
		else
		{
			
		}
		
		/*	give the control to the OS	*/
			vTaskDelay(LED_TASK_PERIODICITY);	
		
		
	}
}

/****************************************************************************
*
*   @brief function acts as task which monitors button 0
*   this function takes pvParameters and return void 
*   @params  name : void
*
*   @return void
****************************************************************************/
void req2_Button0_Task( void * pvParameters)
{
	
	
	EventBits_t aEventBits_ButtonStatusflags;
	uint8 button_status=LOW;
			
	
	while(1)
	{
		
  
  
		 /*	get status flags for button	*/
		aEventBits_ButtonStatusflags = xEventGroupGetBits( gEventGroupHandle_Buttons );

		
		/*   check if the button1 not take the monitoring */
		if ( READ_BIT_BYTE(aEventBits_ButtonStatusflags,BUTTON1_STATUS_FLAG)  == LOW)
		{
			button_status = BTN_Read_Undebounce(BTN0);
			/*		check for button 0 changing event	*/
			if (button_status != READ_BIT_BYTE(aEventBits_ButtonStatusflags,BUTTON0_STATUS_FLAG) )
			{
					
				/* check if the change to high or to low */
				if (button_status == HIGH)
				{
					xEventGroupSetBits(
					gEventGroupHandle_Buttons,    /* The event group being updated. */
					BUTTON0_STATUS_FLAG  );
						

				}
				else
				{
					xEventGroupClearBits(
					gEventGroupHandle_Buttons,    /* The event group being updated. */
					BUTTON0_STATUS_FLAG);
						
				}
				/* inform LCD and led that there is a change*/
				xEventGroupSetBits(
				gEventGroupHandle_Buttons,    /* The event group being updated. */
				BUTTONS_CHANGING_STATUS_LED_FLAG |	BUTTONS_CHANFING_STATUS_LCD_FLAG  );
		}
				}
				else
				{
					
				}
			
		
		vTaskDelay(BUTTON0_TASK_PERIODICITY);
		
	}
}


/****************************************************************************
*
*   @brief function acts as task which monitors button 1
*   this function takes pvParameters and return void
*   @params  name : void
*
*   @return void
****************************************************************************/
void req2_Button1_Task( void * pvParameters)
{
	EventBits_t aEventBits_ButtonsStatusFlags;
	uint8 button_status=LOW;			
	while(1)
	{
	 

   /*	get status flags for buttons	*/
  aEventBits_ButtonsStatusFlags = xEventGroupGetBits( gEventGroupHandle_Buttons );

			/*	check if the button 0 not taking the monitoring	*/
		if ( READ_BIT_BYTE(aEventBits_ButtonsStatusFlags,BUTTON0_STATUS_FLAG) ==LOW)
		{
			button_status = BTN_Read_Undebounce(BTN1);
			
			/*	check for changing in button 0	*/
			if (button_status != READ_BIT_BYTE(aEventBits_ButtonsStatusFlags,BUTTON1_STATUS_FLAG) )
			{
				
				
				/*	detect the change to high or to low	*/
				if (button_status == HIGH)
				{
					xEventGroupSetBits(
					gEventGroupHandle_Buttons,    /* The event group being updated. */
					BUTTON1_STATUS_FLAG  );
						

				}
				else
				{
					xEventGroupClearBits(
					gEventGroupHandle_Buttons,    /* The event group being updated. */
					BUTTON1_STATUS_FLAG);
						
				}
				/* inform LCD and led that there is a change*/
				xEventGroupSetBits(
				gEventGroupHandle_Buttons,    /* The event group being updated. */
				BUTTONS_CHANGING_STATUS_LED_FLAG | BUTTONS_CHANFING_STATUS_LCD_FLAG  );
		}
				}
				else
				{
					
				}
			
		
		vTaskDelay(BUTTON0_TASK_PERIODICITY);
		
	}
}


/****************************************************************************
*
*   @brief function acts as task which monitors buttons and print on the LCD
*   this function takes pvParameters and return void
*   @params  name : void
*
*   @return void
****************************************************************************/
void req2_Lcd_Task( void * pvParameters)
{
	
	
	
	uint8 au8_State=LCD_DISPLAY_STATE_WAIT_LCD_EVENT_BIT;
	char *astr_LedStatus=NULL;
	char *astr_Button_Status=NULL;
	EventBits_t uxBits,EventBits_ButtonsStatusFlags;
	while(TRUE)
	{
		
		
		switch(au8_State)
		{
			/*	wait here until a flag from a button that there is a change	*/
			case LCD_DISPLAY_STATE_WAIT_LCD_EVENT_BIT:
				uxBits= xEventGroupWaitBits(
				gEventGroupHandle_Buttons,   /* The event group being tested. */
				BUTTONS_CHANFING_STATUS_LCD_FLAG, /* The bits within the event group to wait for. */
				pdTRUE,        /*  BIT_4 should be cleared before returning. */
				pdFALSE,       /* Don't wait for both bits, either bit will do. */
				UINT16_MAX );/* Wait a maximum of 100ms for either bit to be set. */
				
				/*  read buttons status to detect which button make the event */
				EventBits_ButtonsStatusFlags= xEventGroupGetBits(gEventGroupHandle_Buttons);
				if ( (uxBits & BUTTONS_CHANFING_STATUS_LCD_FLAG) != FALSE)
				{
					au8_State= LCD_DISPLAY_STATE_CLEAR_AREA ;
					/*	detect which button which makes the leds on	*/
					if ( ((EventBits_ButtonsStatusFlags & BUTTON0_STATUS_FLAG) != FALSE)   )
					{
						astr_LedStatus =" LED ON";
						astr_Button_Status="BUTTON 0 Pressed";
					
					}
					else if ( (EventBits_ButtonsStatusFlags & BUTTON1_STATUS_FLAG) != FALSE)
					{
						astr_LedStatus =" LED ON";
						astr_Button_Status="BUTTON 1 Pressed";	
						
					}
					else
					{
						astr_LedStatus =" LED Off";
						astr_Button_Status="NULL";
					}
				
				}
				else
				{
					
				}
						
					break;
			
			/*	clear displaying area	*/
			case LCD_DISPLAY_STATE_CLEAR_AREA:
				if( ( LCD_Clear()==SEND_CMD_NOT_FINISHED ))
				{
					vTaskDelay(NUMBER_ONE);
				
				}
				else
				{
					au8_State =LCD_DISPLAY_STATE_GOTO_ROW_LOCATION_LED_STATUS;
				
				}
			break;
			/*	go to location to display	*/ 
			case LCD_DISPLAY_STATE_GOTO_ROW_LOCATION_LED_STATUS:
				if( ( LCD_GoToRowColumn(LCD_START_ROW_LED_STATUS,LCD_START_COLUMN_LED_STATUS) == SEND_CMD_NOT_FINISHED))
				{
					vTaskDelay(NUMBER_ONE);
				
				}
				else
				{
					au8_State =LCD_DISPLAY_STATE_DISPLAY_STRING_LED_STATUS;
				}
				break;
				/*		display on LCD		*/
			case LCD_DISPLAY_STATE_DISPLAY_STRING_LED_STATUS:
				if( ( LCD_DisplayString(astr_LedStatus) == SEND_DATA_NOT_FINISHED))
				{
					vTaskDelay(NUMBER_ONE);
				
				}
				else
				{
							/* We have finished accessing the shared resource.  Release the
							semaphore. */
					 
							au8_State =LCD_DISPLAY_STATE_GOTO_ROW_LOCATION_BUTTON_STATUS;
							vTaskDelay(LCD_TASK_PERIODICITY);
				}
			break;
				
				
			case LCD_DISPLAY_STATE_GOTO_ROW_LOCATION_BUTTON_STATUS:
				if( ( LCD_GoToRowColumn(LCD_START_ROW_BUTTON_STATUS,LCD_START_COLUMN_BUTTON_STATUS) == SEND_CMD_NOT_FINISHED))
				{
					vTaskDelay(NUMBER_ONE);
				
				}
				else
				{
				
					au8_State =LCD_DISPLAY_STATE_DISPLAY_STRING_BUTTON_STATUS;
				}
			break;
			/*		display button status on LCD		*/
			case LCD_DISPLAY_STATE_DISPLAY_STRING_BUTTON_STATUS:
				if( ( LCD_DisplayString(astr_Button_Status) == SEND_DATA_NOT_FINISHED))
				{
					vTaskDelay(NUMBER_ONE);
				
				}
				else
				{
					 
						  au8_State =LCD_DISPLAY_STATE_WAIT_LCD_EVENT_BIT;
						  vTaskDelay(LCD_TASK_PERIODICITY);
					
				}
				 
			break;
			
			
		}
		vTaskDelay(LCD_TASK_PERIODICITY);
		
	}

}


/****************************************************************************
*
*   @brief function acts as task which inits the system
*   this function takes pvParameters and return void
*   @params  name : void
*
*   @return void
****************************************************************************/
void req2_Init_Task(void * pvParameters)
{
	
	/*clear led and lcd flags flags*/
	xEventGroupClearBits(
	gEventGroupHandle_Buttons,
	BUTTONS_CHANGING_STATUS_LED_FLAG | BUTTONS_CHANFING_STATUS_LCD_FLAG );
	BTN_init();
	Leds_Init();
	while(TRUE)
	{
		
		/* execuit this until it finishs */
		if (LCD_init() == INIT_LCD_NOT_FINISHED)
		{
			vTaskDelay(NUMBER_ONE);
		}
		else
		{
			
			
			  vTaskResume(req2_Button0_TaskHandler);
			  vTaskResume(req2_Button1_TaskHandler);
			  vTaskSuspend(req2_Init_TaskHandler);
			
			
			
		}
		
		
		
	}
}



/****************************************************************************
*
*   @brief function creates tasks
*   this function takes errorBuffer to fill with the status of the tasks creation and returns status of the creation
*   @params  name : errorBuffer
*	type :uint8 *
*   @return uint8
****************************************************************************/
uint8 req2_tasksCreation(uint8 *errorBuffer)
{
	uint8 status=REQ2_TASKS_SUCCFULLY_CREATED;
	if (errorBuffer != NULL)
	{
	
	errorBuffer[NUMBER_ZERO] =	xTaskCreate(    req2_Init_Task,
						 "Init task",
						 REQ2_LED_INIT_TASK_STACK_DEPTH,
						 REQ2_INPUT_PARAM_REF,
						 REQ2_INIT_TASK_PRIORTIY,
						 &req2_Init_TaskHandler
						 );
	 
	 
			 /*create toggle led task*/
	errorBuffer[NUMBER_ONE]	=xTaskCreate(    req2_Led_Task,
					 "led task",
					 REQ2_LED_TOGGLE_TASK_STACK_DEPTH,
					 REQ2_INPUT_PARAM_REF,
					 REQ2_LED_TOGGLE_TASK_PRIORTY,
					 &req2_LedToggle_TaskHandler
					 );
					 
	 errorBuffer[NUMBER_TWO]	=xTaskCreate(    req2_Button0_Task,
					 "BUTTON 0",
					 REQ2_BUTTON0_TASK_STACK_DEPTH,
					 REQ2_INPUT_PARAM_REF,
					 REQ2_BUTTON0_TASK_PRIORTY,
					 &req2_Button0_TaskHandler
					 );
	errorBuffer[NUMBER_THREE]	=xTaskCreate(    req2_Button1_Task,
					 "BUTTON 1",
					 REQ2_BUTTON1_TASK_STACK_DEPTH,
					 REQ2_INPUT_PARAM_REF,
					 REQ2_BUTTON1_TASK_PRIORTY,
					 &req2_Button1_TaskHandler
					 );
   errorBuffer[NUMBER_FOUR]	=xTaskCreate(    req2_Lcd_Task,
					 "LCD",
					 REQ2_LCD_TASK_STACK_DEPTH,
					 REQ2_INPUT_PARAM_REF,
					 REQ2_LCD_TASK_PRIORTY,
					 &req2_Lcd_TaskHandler
					 );
			for (uint8 index=NUMBER_ZERO;index < REQ2_TASKS_COUNTS_ ;index++ )
			{
				if (errorBuffer[index] !=pdPASS)
				{
					errorBuffer= REQ2_TASKS_NOTSUCCFULLY_CREATED;
				}
			}
			gEventGroupHandle_Buttons=xEventGroupCreate();
			vTaskSuspend(req2_Button0_TaskHandler);
			vTaskSuspend(req2_Button1_TaskHandler);
			
					 
		}
		else
		{
			status =REQ2_TASKS_ERROR_REPORT_BUFFER_NULL;
		}
	return status;
}


