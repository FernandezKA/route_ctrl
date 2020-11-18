#include "inc.h"
/*I2C config*/
RING uart;
RING i2c;
unsigned char temp = 0U;
command *CMD = new command;
/*******************************************************************************/
int main(void)
{
  SystemInit();
  
  while (1)
  {
    if(CMD->get_status()){
       GPIO_WriteLow(GPIOA, GPIO_PIN_1);
    }
    else{
       GPIO_WriteHigh(GPIOA, GPIO_PIN_1);
    }

    if (BitMask(UART1->SR, 1U << 7))   { /*waiting while txe==1*/
      if (!i2c.isEmpty()) UART1->DR = i2c.pull();
    }
  }
}
