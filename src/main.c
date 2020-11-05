#include "inc.h"
/*I2C config*/
uint8_t address = 0x6eU; /*default value*/
bool started = 0;
/*Variable init*/
RING uart;
RING i2c;
unsigned char temp = 0U;
/*******************************************************************************/
int main(void)
{
  SystemInit();
  while (1)
  {
    if(uart.inp>uart.out){
       GPIO_WriteLow(GPIOA, GPIO_PIN_1);
    }
    else{
       GPIO_WriteHigh(GPIOA, GPIO_PIN_1);
    }
    while (i2c.inp != i2c.out)
    {
      if (BitMask(UART1->SR, 1U << 7))
      { /*waiting while txe==1*/
        UART1->DR = i2c.pull();
      }
    }
  }
}
