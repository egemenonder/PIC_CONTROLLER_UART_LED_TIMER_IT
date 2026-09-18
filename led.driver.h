#ifndef LED_DRIVER_H
#define LED_DRIVER_H


#define LED1 PIN_E4

#define LED_OFF   0
#define LED_ON    1
#define LED_BLINK 2

typedef struct
{
   int CaseState;
} Timer2;

void SetTimer2State(TimerStateMode state);

void Led_ac(void);
void Led_kapa(void);
void Led_yan_son(void);

int LED_GetCaseState(void);

#endif
