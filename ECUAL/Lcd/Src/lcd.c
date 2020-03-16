/****************************************************************************
 * @file  LCD.c
 * @brief this file includes some functions prototype to manipulate LCD
 *
 * @author Abdelmonem Mostafa
 * @date  OCT 2019
 *
 ****************************************************************************/
 
 #include "stdlib.h"

  
  /************************************************************************/
  /*		MACROS  for stating LCD                                        */
  /************************************************************************/
 
  
  
#define INIT_STATE_LCD_DIR_AND_MODE				0
#define INIT_STATE_LCD_MODE						4

#define INIT_STATE_LCD_INFO_SEND				1
#define INIT_STATE_LCD_CURSOR_OFF_SEND			2
#define INIT_STATE_LCD_CLEAR_SEND				3

#define CONVERT_NUMBER_STATE					0
#define PRINTING_STRING_STATE					1

#define SENDING_ROW_COL_STATE					0
#define SENDING_STRING_STATE					1


#define CLEAR_RS_RW_STATE					0
#define SET_RS_CLEAR_RW_STATE				0
#define	SET_ENABLE_STATE_HIGH_NIBBLE		1
#define SET_HIGH_NIBBLE_DATA				2
#define	CLEAR_ENABLE_STATE_HIGH_NIBBLE		3
#define	SET_ENABLE_STATE_LOW_NIBBLE			4
#define SET_LOW_NIBBLE_DATA					5
#define	CLEAR_ENABLE_STATE_LOW_NIBBLE		6
#define	WAIT_CLEAR_ENABLE_STATE_LOW_NIBBLE	7



#define  STATE_GOTO_FIRST_POSITION			0
#define  STATE_CLEARING_AREA_POSITION		1

#define BUFFER_SIZE							10


#include "lcd.h"

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/


/****************************************************************************
*
*   @brief function init LCD
*   this function takes void  return INIT_LCD_FINISHED if it hasn't finished yet else return INIT_LCD_NOT_FINISHED
*   @params  name : void
*
*   @return uint8
****************************************************************************/
uint8 LCD_init(void)
{
	static uint8 u8_InitState = INIT_STATE_LCD_DIR_AND_MODE;
	uint8 InitStatus=INIT_LCD_NOT_FINISHED;
	
	  
	switch(u8_InitState)
	{
		case INIT_STATE_LCD_DIR_AND_MODE  :
			Gpio_PinDirection(LCD_CTL_PORT,(1<<E) | (1<<RS) | (1<<RW),OUTPUT);
			Gpio_UpperNibbleDirection(LCD_DAT_PORT,OUTPUT);
			u8_InitState = INIT_STATE_LCD_MODE;
			break;
		case  INIT_STATE_LCD_MODE :
			u8_InitState  =	LCD_SendCommand(FOUR_BITS_DATA_MODE) == SEND_CMD_FINISHED ? INIT_STATE_LCD_INFO_SEND : INIT_STATE_LCD_MODE  ; /* initialize LCD in 4-bit mode */
			break;
		case INIT_STATE_LCD_INFO_SEND : 
			u8_InitState =	LCD_SendCommand(TWO_LINE_LCD_Four_BIT_MODE)== SEND_CMD_FINISHED ? INIT_STATE_LCD_CURSOR_OFF_SEND : INIT_STATE_LCD_INFO_SEND  ; /* use 2-line lcd + 4-bit Data Mode + 5*7 dot display Mode */
			break; 
		case INIT_STATE_LCD_CURSOR_OFF_SEND:
			u8_InitState= LCD_SendCommand(CURSOR_OFF) == SEND_CMD_FINISHED ? INIT_STATE_LCD_CLEAR_SEND : INIT_STATE_LCD_CURSOR_OFF_SEND  ; /* cursor off */
			break;
		case INIT_STATE_LCD_CLEAR_SEND :
			u8_InitState = LCD_SendCommand(CLEAR_COMMAND) == SEND_CMD_FINISHED ? INIT_STATE_LCD_DIR_AND_MODE : INIT_STATE_LCD_CLEAR_SEND ; /* clear LCD at the beginning */
			InitStatus=  (u8_InitState == INIT_STATE_LCD_DIR_AND_MODE) ? INIT_LCD_FINISHED :INIT_LCD_NOT_FINISHED;
			break;
		default:
		 break;
	}
	return InitStatus;
}




/****************************************************************************
*
*   @brief function send data  to LCD
*   this function takes port command and return SEND_CMD_NOT_FINISHED if it hasn't finished yet else return SEND_CMD_FINISHED
*   @params  name : cmd
*			 type :uint8
*
*   @return uint8
****************************************************************************/
uint8 LCD_SendCommand(uint8 cmd)
{
	
	static uint8 lsu8_CmdState = CLEAR_RS_RW_STATE;
	uint8 sendCmdStatus=SEND_CMD_NOT_FINISHED;
	
	
		switch(lsu8_CmdState)
		{
			case CLEAR_RS_RW_STATE :
				Gpio_PinWrite_Same_Value(LCD_CTL_PORT,1<<RS,LOW);
				Gpio_PinWrite_Same_Value(LCD_CTL_PORT,1<<RW,LOW);
				
				break;
			case SET_ENABLE_STATE_HIGH_NIBBLE:
				Gpio_PinWrite_Same_Value(LCD_CTL_PORT,1<<E,HIGH);
				
				break;
			case SET_HIGH_NIBBLE_DATA :
				Gpio_UpperNibbleWrite(LCD_DAT_PORT,(cmd&0xf0 )>>4);
				
				break;
			case CLEAR_ENABLE_STATE_HIGH_NIBBLE :
				Gpio_PinWrite_Same_Value(LCD_CTL_PORT,1<<E,LOW); /* disable LCD E=0 */
				
				break;
			case SET_ENABLE_STATE_LOW_NIBBLE:
				Gpio_PinWrite_Same_Value(LCD_CTL_PORT,1<<E,HIGH); /* Enable LCD E=1 */
				
				break;
			case SET_LOW_NIBBLE_DATA:
			/* out the lowest 4 bits of the required command to the data bus D4 --> D7 */
				Gpio_UpperNibbleWrite(LCD_DAT_PORT,(cmd&0x0f ));
				
				break;
			case CLEAR_ENABLE_STATE_LOW_NIBBLE:
				Gpio_PinWrite_Same_Value(LCD_CTL_PORT,1<<E,LOW); /* disable LCD E=0 */
				break;
			case WAIT_CLEAR_ENABLE_STATE_LOW_NIBBLE :
				lsu8_CmdState =-1;
				sendCmdStatus=SEND_CMD_FINISHED;
				
				break;
			default:
				/*do nothing*/
				
				break;
			
		
		}
		++lsu8_CmdState;
	
	return sendCmdStatus;
}




/****************************************************************************
*
*   @brief function send data to LCD
*   this function takes port data and return SEND_DATA_NOT_FINISHED if it hasn't finished yet else return SEND_DATA_FINISHED
*   @params  name : cmd
*			 type :uint8
*   @params  name : direction
*			 type :uint8
*
*   @return void
****************************************************************************/
uint8 LCD_DisplayChar(uint8 data)
{
	
	
	static uint8 lsu8_DataState =CLEAR_RS_RW_STATE;
	uint8 sendDataStatus = SEND_DATA_NOT_FINISHED;
		
		switch(lsu8_DataState)
		{
			case SET_RS_CLEAR_RW_STATE :
				Gpio_PinWrite_Same_Value(LCD_CTL_PORT,1<<RS,HIGH);
				Gpio_PinWrite_Same_Value(LCD_CTL_PORT,1<<RW,LOW);
			
				break;
			case SET_ENABLE_STATE_HIGH_NIBBLE:
				Gpio_PinWrite_Same_Value(LCD_CTL_PORT,1<<E,HIGH);
			
				break;
				
			case SET_HIGH_NIBBLE_DATA :
				Gpio_UpperNibbleWrite(LCD_DAT_PORT,(data&0xf0 )>>4);
			
				break;
			case CLEAR_ENABLE_STATE_HIGH_NIBBLE :
				Gpio_PinWrite_Same_Value(LCD_CTL_PORT,1<<E,LOW); /* disable LCD E=0 */
			
				break;
			case SET_ENABLE_STATE_LOW_NIBBLE:
				Gpio_PinWrite_Same_Value(LCD_CTL_PORT,1<<E,HIGH); /* Enable LCD E=1 */
			
				break;
			case SET_LOW_NIBBLE_DATA:
				/* out the lowest 4 bits of the required command to the data bus D4 --> D7 */
				Gpio_UpperNibbleWrite(LCD_DAT_PORT,(data&0x0f ));
			
				break;
			case CLEAR_ENABLE_STATE_LOW_NIBBLE:
				Gpio_PinWrite_Same_Value(LCD_CTL_PORT,1<<E,LOW); /* disable LCD E=0 */
				break;
			case WAIT_CLEAR_ENABLE_STATE_LOW_NIBBLE:
				sendDataStatus = SEND_DATA_FINISHED;
				lsu8_DataState=-1;
			
			break;
				default:
				/*do nothing*/
				
				
			break;
			
			
		}
		++lsu8_DataState;

	
	return sendDataStatus;
	
}



/****************************************************************************
*
*   @brief function display string on LCD
*   this function takes port command and return PRINT_STRING_LCD_FINISHED if it hasn't finished yet else return PRINT_STRING_LCD_NOT_FINISHED
*   @params  name : cmd
*			 type :uint8
*
*   @return uint8
****************************************************************************/
uint8 LCD_DisplayString(const char *Str)
{
	static uint8 i = 0;
	uint8 print_State =PRINT_STRING_LCD_NOT_FINISHED;
	if ((Str[i] != '\0'))
	{
	 if (LCD_DisplayChar(Str[i]) == SEND_DATA_FINISHED)
	 {
		 i++;
	 }
	 else
	 {
		//nothing
	 }
	 	
		
	}
	else
	{
		print_State =PRINT_STRING_LCD_FINISHED;
		i=0;
	}
	return print_State;
}


/****************************************************************************
*
*   @brief function display string on LCD
*   this function takes port command and return SEND_CMD_LCD_FINISHED if it hasn't finished yet else return SEND_CMD_LCD_NOT_FINISHED
*   @params  name : cmd
*			 type :uint8
*
*   @return uint8
****************************************************************************/

uint8 LCD_GoToRowColumn(uint8 row,uint8 col)
{
	uint8_t firstCharAdress[]={0x80,0xc0,0x94,0xD4};
	return LCD_SendCommand(firstCharAdress[row-1]+col-1);
	//LCD_SendCommand(Address | SET_CURSOR_LOCATION); 
}



/****************************************************************************
*
*   @brief function display string on LCD
*   this function takes port command and return PRINT_STRING_LCD_FINISHED if it hasn't finished yet else return PRINT_STRING_LCD_NOT_FINISHED
*   @params  name : cmd
*			 type :uint8
*
*   @return uint8
****************************************************************************/
uint8 LCD_printStringRowColumn(uint8 row,uint8 col,const char *Str)
{
	static uint8 state =0;
	uint8 status=PRINT_STRING_LCD_NOT_FINISHED;
	
	switch(state)
	{
		case SENDING_ROW_COL_STATE:
			if( LCD_GoToRowColumn(row,col) == SEND_CMD_FINISHED) /* go to to the required LCD position */
			{
				state = SENDING_STRING_STATE;	
			}
		break;
		case SENDING_STRING_STATE:
			 /* display the string */
			if( LCD_DisplayString(Str) == PRINT_STRING_LCD_FINISHED) 
			{
				state =SENDING_ROW_COL_STATE;
				status =PRINT_STRING_LCD_FINISHED;
			}
			break;
	}
	return status;
}

/****************************************************************************
*
*   @brief function display numbers as string on LCD
*   this function takes port command and return PRINT_STRING_LCD_FINISHED if it hasn't finished yet else return PRINT_STRING_LCD_NOT_FINISHED
*   @params  name : cmd
*			 type :uint8
*
*   @return uint8
****************************************************************************/
uint8 LCD_IntToString(int data)
{
	static uint8 state = CONVERT_NUMBER_STATE;
	uint8 status = INT_TO_STRING_LCD_NOT_FINISHED;
	static char buff[BUFFER_SIZE]; /* String to hold the ascii result */
	switch (state)
	{
		case CONVERT_NUMBER_STATE :
		   itoa(data,buff,10); /* 10 for decimal */
		   state = PRINTING_STRING_STATE;
		case PRINTING_STRING_STATE :
			 if (LCD_DisplayString(buff) == PRINT_STRING_LCD_FINISHED)
			 {
				 state =CONVERT_NUMBER_STATE;
				 status = INT_TO_STRING_LCD_FINISHED;
			 }
		   break;
		 default:
		   break;
	}
	return status;
}


/****************************************************************************
*
*   @brief function Clear LCD
*   this function takes  void and return SEND_CMD_LCD_FINISHED if it hasn't finished yet else return SEND_CMD_LCD_NOT_FINISHED
*   @params  name : cmd
*			 type :uint8
*
*   @return uint8
****************************************************************************/
uint8 LCD_Clear(void)
{
	return LCD_SendCommand(CLEAR_COMMAND); //clear display 
}


/****************************************************************************
*
*   @brief function Clear LCD Area
*   this function takes start place(row,col) and number of spaces to be cleared 
*	and return SEND_CMD_LCD_FINISHED if it hasn't finished yet else return SEND_CMD_LCD_NOT_FINISHED
*   @params  name :		row
*			 type :		uint8
*   @params  name :		 col
*			 type :		uint8
**   @params name :		spaces_count
*			 type :		uint8
*   @return uint8
****************************************************************************/

uint8 LCD_ClearArea(uint8 row,uint8 col,uint8 spaces_count)
{
	uint8 status =LCD_CLEAR_AREA_NOTFINISED;
	static uint8 state=STATE_GOTO_FIRST_POSITION;
	static uint8 ls8_count;
	
	switch(state)
	{
		case STATE_GOTO_FIRST_POSITION:
		state= LCD_GoToRowColumn(row,col) == SEND_CMD_FINISHED ? STATE_CLEARING_AREA_POSITION : STATE_GOTO_FIRST_POSITION ;
		ls8_count = spaces_count;
		break;
		case STATE_CLEARING_AREA_POSITION:
		if (ls8_count == 0)
		{
			status=LCD_CLEAR_AREA_FINISED;
			state=STATE_GOTO_FIRST_POSITION;
		}
		else if (LCD_DisplayChar(' ') == SEND_DATA_FINISHED)
		{
			--ls8_count;
		}
		
		
		break;
	}
	return status;
}