

#include <avr/io.h>

#include "task.h"
#include "Req2_LedLcd.h"
#include "User_stroy_2.h"
#include "led.h"


int main(void)
{
    Leds_Init();
	
	 uint8 arr[REQ2_TASKS_COUNTS];
	 
	 
	req2_tasksCreation(arr);
	 vTaskStartScheduler();
    while (1)
    {
    }
}

