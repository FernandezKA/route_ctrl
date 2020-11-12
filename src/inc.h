#ifndef _inc_h_
#define _inc_h_
#include "stm8s_conf.h"
/***********************/
#define BitMask(a, b) (((a) & (b)) == (b))
#define BUFF_SIZE (64U)
#define DEF_ADDR (0x34U)
/***********************/
extern bool started;
extern unsigned char temp;
/***********************/
struct RING
{
public:
  unsigned char inp;
  unsigned char out;
  unsigned char data[BUFF_SIZE];
  RING()
  {
    inp = out = 0U;
  }
void push(const unsigned char temp)
  {
    if (inp == BUFF_SIZE - 1U)
    {
      inp = 0U;
    }
    data[inp++] = temp;
  }
unsigned char pull(void)
  { 
    if(out>inp){
       out = inp;
    }
    if(out==BUFF_SIZE-1U){
       out = inp = 0x00U;
    }
    return data[out++];
  }
};
extern RING uart;
extern RING i2c;
/***************************************************************************************/
void SystemInit(void);
uint8_t dev_addr(void); /*Функция, определяющая адрес устройства*/
void gpio_config(void);
void i2c_init(void);

#endif