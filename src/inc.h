#ifndef _inc_h_
#define _inc_h_
#include "stm8s_conf.h"
/***********************/
#define BitMask(a, b) (((a) & (b)) == (b))
#define BUFF_SIZE (64U)
#define DEV_ADDR (0x38U)

/***********************/
extern bool started;
extern unsigned char result;
extern bool unread;
extern unsigned char counter;
extern unsigned char uc_i2c_size;
/*************************************************************************/

class I2C_buff
{
private:
  bool repeat;
  bool completed;
  uint8_t size;
  uint8_t countSend;
  uint8_t countRecieve;
  uint8_t command[BUFF_SIZE];

public:
  explicit I2C_buff(void)
  {
    repeat = false;
    completed = false;
    countSend = 0x00U;
    countRecieve = 0x00U;
    size = 0x00U;
  }
  inline bool I2C_state(void) const
  {
    return repeat;
  }
  inline bool getComplete(void) const
  {
    return completed;
  }
inline void endRecieve(void){
  completed = true;
  if(repeat){
    TIM1->CR1|=TIM1_CR1_CEN;
  }
  else if(!repeat){
    getPull();
  }
}
  inline void getParse(const uint8_t &new_value)
  {
    switch (countRecieve)
    {
    case 0x00U:
      switch (new_value)
      {
      case 0x00U:
        repeat = false;
        rIncrement();
        break;

      case 0x01:
        repeat = true;
        rIncrement();
        break;

      default:
        repeat = false;
        I2C_buff(); /*reinit buff*/
        break;
      }
      break;

    case 0x01U:
      switch (new_value)
      {
      case 0x00U:
        I2C_buff(); /*reinit*/
        break;

      default:
        size = new_value; /*add recieved size of packet*/
        break;
      }
      break;

    default:
      pushBack(new_value);
      break;
    }
  }
  inline void getPull(void)
  {
      while(countSend < size - 1){
      while(~UART1_SR_TXE){asm("nop");}
      UART1->DR = command[countSend];
    }
  }
  inline void pushBack(const uint8_t &value)
  {
    command[countRecieve] = value;
    rIncrement();
  }
  inline void rIncrement(void)
  {
    if (countRecieve < size)
    {
      ++countRecieve;
    }
    else
    {
      completed = true;
      countRecieve = 0x00U;
    }
  }
};


extern I2C_buff ibuff;
/*********************************************************************/
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
    if (isFull())
      return;
    data[inp++] = temp;
    if (inp == BUFF_SIZE)
      inp = 0U;
    count++;
    //    if (count == BUFF_SIZE - 1U) full = true;
  }
  inline unsigned char pull(void)
  {
    register unsigned char o1;

    if (isEmpty())
    {
      rollback_disable = true;
      return 0;
    }

    o1 = data[out++];
    if (out == BUFF_SIZE)
      out = 0U;
    --count;
    return o1;
  }
  inline void rollback(void) // rolls back buffer receiver pointer one byte backward
  {
    if (rollback_disable)
    {
      rollback_disable = false;
      return;
    }
    rollback_disable = false;
    if (out == 0)
      out = BUFF_SIZE;
    out--;
    count++;
  }
  inline bool isEmpty(void)
  {
    if (count == 0)
      return true;
    else
      return false;
  }

  inline bool isFull(void)
  {
    return (count == (BUFF_SIZE - 1U));
  }

  inline unsigned char getCount(void)
  {
    return count;
  };
};
extern RING *uart;
extern RING *i2c;
/***************************************************************************************/
void SystemInit(void);
uint8_t dev_addr(void); /*Функция, определяющая адрес устройства*/
void gpio_config(void);
void i2c_init(void);
/***************************************************************************************/

//extern unsigned char lastCmdSize;
//unsigned char getLastSize(void);
//unsigned char processPacketData(unsigned char cb);

#endif