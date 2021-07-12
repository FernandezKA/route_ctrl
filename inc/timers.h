#ifndef _timers_h
#define _timers_h
#include "inc.h"
void __vTim1Config(void);
INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_BRK_IRQHandler, 11);
#endif