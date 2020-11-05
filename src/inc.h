#ifndef _inc_h_
#define _inc_h_
#include "stm8s_conf.h"
/***********************/
#define BitMask(a, b) (((a) & (b)) == (b))
/***********************/
extern uint8_t address;
extern bool started;
extern unsigned char temp;
/***********************/
struct RING{
  public:
  unsigned char inp;
  unsigned char out;
  unsigned char data[64U];
  RING(){
     inp = out = 0U;
  }
  void push(const unsigned char temp){
    if(inp == 63U){
      inp = 0U;
    }
      data[inp++]=temp;
  }
  unsigned char pull(void){
    if(out == 63U){
    out = 0U;
    }
      return data[out++];
  }
};
extern RING uart;
extern RING i2c;
/***************************************************************************************/
void SystemInit(void);
uint8_t dev_addr(void);										  /*Функция, определяющая адрес устройства*/
void gpio_config(void);
void i2c_init(const unsigned char& addr);

#endif