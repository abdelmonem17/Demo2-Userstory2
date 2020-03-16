 /******************************************************************************
 *
 * Module: LCD
 *
 * File Name: lcd.h
 *
 * Description: Header file for the LCD driver
 *
 * 
 *
 *******************************************************************************/

#ifndef LCD_H_
#define LCD_H_
#include "avr/io.h"
#include "types.h"
#include "gpio.h"
#define F_CPU 8000000UL
#include "util/delay.h"


/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/
/* LCD Data bits mode configuration */
#define DATA_BITS_MODE 4

/* Use higher 4 bits in the data port */
#if (DATA_BITS_MODE == 4)
#define UPPER_PORT_PINS
#endif

/* LCD HW Pins */
#define RS 1
#define RW 2
#define E  3
#define LCD_CTRL_PORT PORTA
#define LCD_CTRL_PORT_DIR DDRA
#define LCD_CTL_PORT	PORT_A

#define LCD_DATA_PORT PORTA
#define LCD_DATA_PORT_DIR DDRA

#define LCD_DAT_PORT	PORT_A

/* LCD Commands */
#define CLEAR_COMMAND 0x01
#define FOUR_BITS_DATA_MODE 0x02
#define TWO_LINE_LCD_Four_BIT_MODE 0x28
#define TWO_LINE_LCD_Eight_BIT_MODE 0x38
#define CURSOR_OFF 0x0C
#define CURSOR_ON 0x0E
#define SET_CURSOR_LOCATION 0x80 

#define CLEAR_BIT(a,b)	CLR_BIT(a,b)	



 #define SEND_CMD_FINISHED							1
 #define SEND_CMD_NOT_FINISHED						0
 
 #define SEND_DATA_FINISHED							1
 #define SEND_DATA_NOT_FINISHED						0
 
 #define INIT_LCD_FINISHED							1
 #define INIT_LCD_NOT_FINISHED						0
 
  #define PRINT_STRING_LCD_FINISHED					1
  #define PRINT_STRING_LCD_NOT_FINISHED				0
  
  #define INT_TO_STRING_LCD_FINISHED					1
  #define INT_TO_STRING_LCD_NOT_FINISHED				0
  
  #define CLEAR_AREA_LCD_FINISHED					1
  #define CLEAR_AREA_LCD_NOT_FINISHED				0
  
  #define  LCD_CLEAR_AREA_FINISED				1
  #define  LCD_CLEAR_AREA_NOTFINISED			0
/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
uint8 LCD_SendCommand(uint8 cmd);
uint8 LCD_DisplayChar(uint8 data);
uint8 LCD_DisplayString(const char *Str);
uint8 LCD_init(void);

/****************************************************************************
*
*   @brief function Clear LCD
*   this function takes port command and return SEND_CMD_LCD_FINISHED if it hasn't finished yet else return SEND_CMD_LCD_NOT_FINISHED
*   @params  name : cmd
*			 type :uint8
*
*   @return uint8
****************************************************************************/
uint8 LCD_Clear(void);
/****************************************************************************
*
*   @brief function display string on LCD
*   this function takes port command and return PRINT_STRING_LCD_FINISHED if it hasn't finished yet else return PRINT_STRING_LCD_NOT_FINISHED
*   @params  name : cmd
*			 type :uint8
*
*   @return uint8
****************************************************************************/
uint8 LCD_printStringRowColumn(uint8 row,uint8 col,const char *Str);
/****************************************************************************
*
*   @brief function display string on LCD
*   this function takes port command and return SEND_CMD_LCD_FINISHED if it hasn't finished yet else return SEND_CMD_LCD_NOT_FINISHED
*   @params  name : cmd
*			 type :uint8
*
*   @return uint8
****************************************************************************/
uint8 LCD_GoToRowColumn(uint8 row,uint8 col);
/****************************************************************************
*
*   @brief function display numbers as string on LCD
*   this function takes port command and return PRINT_STRING_LCD_FINISHED if it hasn't finished yet else return PRINT_STRING_LCD_NOT_FINISHED
*   @params  name : cmd
*			 type :uint8
*
*   @return uint8
****************************************************************************/
uint8 LCD_IntToString(int data);


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
uint8 LCD_ClearArea(uint8 startRow,uint8 endRow,uint8 col);

#endif /* LCD_H_ */
