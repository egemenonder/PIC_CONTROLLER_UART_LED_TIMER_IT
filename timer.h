#ifndef TIMER_H
#define TIMER_H

#define EPOS_CIT_QUE_TIMER_WRITE_VAL  27344
#define EPOS_CIT_QUE_DIV_VAL          TMR_DIV_BY_256
#define EPOS_CIT_QUE_TIMER_CHUNK      100

typedef enum
{
   TIMER_CREATE = 0,
   TIMER_OPEN,
   TIMER_STOP
} TimerStateMode;

#endif
