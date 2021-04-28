#include "inc.h"
/*I2C config*/
RING *uart = new RING;
RING *i2c = new RING;
unsigned char result = 0U;
bool unread = false;
unsigned char counter = 0U;
/*******************************************************************************/
int main(void)
{
  SystemInit();

  while (1)
  {
    if (unread)
    {
      GPIO_WriteLow(GPIOA, GPIO_PIN_1);
      GPIOD->ODR|=(1U<<3);
    }
    else
    {
      GPIO_WriteHigh(GPIOA, GPIO_PIN_1);
      GPIOD->ODR&=~(1U<<3);
    }
  
    if (BitMask(UART1->SR, 1U << 7))
    { /*waiting while txe==1*/
      if (!i2c->isEmpty())
        UART1->DR = i2c->pull();
    }
  }
}
