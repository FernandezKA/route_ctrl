#include "inc.h"
/*I2C config*/
RING *uart = new RING;
RING *i2c = new RING;
RING *tmp = new RING;
unsigned char result = 0U;
/*******************************************************************************/
int main(void)
{
  SystemInit();

  while (1)
  {
    if (result!=0U)
    {
      GPIO_WriteLow(GPIOA, GPIO_PIN_1);
    }
    else
    {
      GPIO_WriteHigh(GPIOA, GPIO_PIN_1);
    }

    if (BitMask(UART1->SR, 1U << 7))
    { /*waiting while txe==1*/
      if (!i2c->isEmpty())
        UART1->DR = i2c->pull();
    }
  }
}
