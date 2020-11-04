#include "inc.h"
/*I2C config*/
uint8_t address = 0x6eU; /*default value*/
/*Variable init*/
Ring_buff bf;
RING uart;
/*******************************************************************************/
int main(void)
{
  SystemInit();
  bf.inp = 0x00U;
  bf.output = 0x00U;
  while (1)
  {
    while (bf.inp != bf.output)
    {

      if (BitMask(UART1->SR, 1U << 7))
      {                       /*waiting while txe==1*/
        UART1->DR = pop(&bf); /*send data to DR of uart*/
      }
    }
  }
}
