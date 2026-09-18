#include "setup.h"
#include "timer.h"

#INT_TIMER2
void TIMER_2_INTERRUPT(void)
{
   //if (LED_GetCaseState() == LED_BLINK)
   //{
      output_toggle(LED1);
   //}

   clear_interrupt(INT_TIMER2);
   set_timer2(0);
}

void SetTimer2State(TimerStateMode state)
{
   switch (state)
   {
      case TIMER_CREATE:

         setup_timer2(TMR_INTERNAL | EPOS_CIT_QUE_DIV_VAL, EPOS_CIT_QUE_TIMER_WRITE_VAL);

         disable_interrupts(INT_TIMER2);

         output_low(LED1);

         break;

      case TIMER_OPEN:

         //setup_timer2(TMR_INTERNAL | EPOS_CIT_QUE_DIV_VAL, EPOS_CIT_QUE_TIMER_WRITE_VAL);

         set_timer2(0);

         enable_interrupts(INT_TIMER2);

         break;

      case TIMER_STOP:

         disable_interrupts(INT_TIMER2);

         break;

      default:

         break;
   }
}
