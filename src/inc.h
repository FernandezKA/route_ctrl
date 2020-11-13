#ifndef _inc_h_
#define _inc_h_
#include "stm8s_conf.h"
/***********************/
#define BitMask(a, b) (((a) & (b)) == (b))
#define BUFF_SIZE (64U)
#define DEF_ADDR (0x32U)
/***********************/
extern bool started;
extern unsigned char temp;
/***********************/
struct RING
{
public:
  unsigned char inp;
  unsigned char out;
  unsigned char count;
  bool rollback_disable;
  unsigned char data[BUFF_SIZE];
  RING()
  {
    inp = 0U;
    out = 0U;
    count = 0U;
    rollback_disable = false;
  }
  inline void push(const unsigned char temp)
  {
    if (isFull()) return;
    data[inp++] = temp;
    if (inp == BUFF_SIZE) inp = 0U;
    count++;
//    if (count == BUFF_SIZE - 1U) full = true;
  }
  inline unsigned char pull(void)
  {   
    register unsigned char o1;

    if (isEmpty()) { 
      rollback_disable = true;
      return 0;
    }

    o1 =  data[out++];
    if (out == BUFF_SIZE) out = 0U;
    --count;
    return o1;
  }

  inline void rollback(void) // rolls back buffer receiver pointer one byte backward
  {
    if (rollback_disable) {
      rollback_disable = false;
      return;
    }
    rollback_disable = false;
    if (out == 0) out = BUFF_SIZE;
    out--;
    count++;
  }
  
  inline bool isEmpty(void)
  {
    if (count == 0) return true; else return false;
  }
  
  inline bool isFull(void)
  {
    return (count==(BUFF_SIZE - 1U));
  }

  inline unsigned char getCount(void) 
  { 
    return count;
  };
  
};
extern RING uart;
extern RING i2c;
/***************************************************************************************/
void SystemInit(void);
uint8_t dev_addr(void); /*Функция, определяющая адрес устройства*/
void gpio_config(void);
void i2c_init(void);

#endif