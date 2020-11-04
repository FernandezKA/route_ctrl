#ifndef _inc_h_
#define _inc_h_
#include "stm8s_conf.h"
/***********************/
#define BitMask(a, b) (((a) & (b)) == (b))
/***********************/
extern uint8_t address;
/***********************/
struct RING{
private:
  volatile unsigned char inp;
  volatile unsigned char out;
  volatile unsigned char data[64U];
public:
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
typedef struct
{
	unsigned char buffer[64U];
	unsigned char inp;
	unsigned char output;
} Ring_buff;
extern void put(unsigned char data, Ring_buff *buffer);
extern unsigned char pop(Ring_buff *buff);
extern Ring_buff bf;
extern RING uart;
/***************************************************************************************/
void SystemInit(void);
uint8_t dev_addr(void);										  /*Функция, определяющая адрес устройства*/
void gpio_config(void);
void i2c_init(unsigned char addr);

#endif