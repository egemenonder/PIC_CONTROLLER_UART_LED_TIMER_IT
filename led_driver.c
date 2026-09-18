#include "led_driver.h"

static Timer2 x = {LED_OFF};

void Led_ac(void)
{
   SetTimer2State(TIMER_STOP);
   x.CaseState = LED_ON;
   output_high(LED1);
}

void Led_kapa(void)
{
   SetTimer2State(TIMER_STOP);
   x.CaseState = LED_OFF;
   output_low(LED1);
}

void Led_yan_son(void)
{
   SetTimer2State(TIMER_OPEN);
   x.CaseState = LED_BLINK;
}

int LED_GetCaseState(void)
{
   return x.CaseState;
}
