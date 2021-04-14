#include "inc.h"
/*I2C config*/
RING *uart = new RING; /*ring for uart send*/
RING *i2c = new RING;  /*ring for I2C send*/
unsigned char result = 0U;
bool unread = false;
unsigned char counter = 0U;
I2C_buff ibuff;
/*******************************************************************************/
/*This function generated IRQ for send repeated value at UART*/
static inline void TIM1_Init(void)
{
  TIM1->CR1 |= TIM1_CR1_ARPE; /*ENABLE AUTORELOAD*/
  TIM1->IER |= TIM1_IER_UIE;  /*ENABLE UPDATE INTERRUPT*/
  TIM1->PSCRH = 7999U >> 8;   /*SET PERIOD 8 MHz/2048/255 = 15 Hz => TIME RECIEVE = 66.6 mS*/
  TIM1->PSCRL = 7999U & 0xFFU;
  TIM1->ARRH = 700U >> 8;
  TIM1->ARRL = 700U & 0xFFU;
  
  /**********************IMPORTANT!*********************************************/
  /*AT THIS STATE TIM1 IS DISABLE*/
}
int main(void)
{
  SystemInit();
  TIM1_Init();
  //I2C_buff ibuff;
  while (1)
  {
    if (unread)
    {
      GPIO_WriteLow(GPIOA, GPIO_PIN_1);
    }
    else
    {
      GPIO_WriteHigh(GPIOA, GPIO_PIN_1);
    } 
  }
}
