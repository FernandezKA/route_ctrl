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
extern unsigned char get_count(Ring_buff *buff);
extern void clean(Ring_buff *buff);
extern _Bool IsEmpty(Ring_buff *buff);
extern Ring_buff bf;
/***************************************************************************************/
void SystemInit(void);
void recognize_data(unsigned char data); /*Функция распознавания принятых данных*/
uint8_t dev_addr(void);										  /*Функция, определяющая адрес устройства*/
void gpio_config(void);
void I2C_recognize();
void i2c_init(unsigned char addr);
void I2C_transaction_begin(void);

#endif