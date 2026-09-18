#include "setup.h"
#include "timer.c"
#include "led_driver.c"
#include "uart.c"
#include "pcd_bootloader.h"

void main()
{

   SetTimer2State(TIMER_CREATE);
   //SetTimer2State(TIMER_OPEN);
   SetTimer2State(TIMER_STOP);
   enable_interrupts(GLOBAL);
 
while(TRUE)
   {
    uart_khbit();
    
   }
}
