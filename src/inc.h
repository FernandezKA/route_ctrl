#ifndef _inc_h_
#define _inc_h_
#include "stm8s_conf.h"
/***********************/
#define BitMask(a, b) (((a) & (b)) == (b))
#define BUFF_SIZE (64U)
#define DEF_ADDR (0x32U)
#define SIGNATURE (0x5FU)
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
class command{
private:
  unsigned char signature;
  unsigned char dest_id_1;
  unsigned char dest_id_2;
  unsigned char cmd;
  unsigned char size;
  unsigned char hcrc;
  unsigned char dcrc;
    unsigned char hcrcr;
  unsigned char dcrcr;
  unsigned char data[64U];
  unsigned char index;
  bool completed;
public:
  command (){
     index = 0;
     hcrcr = dcrcr = 0xFFU;
  }
  void buf_clear() {
    for(uint8_t i = 0; i<63U; ++i){
      data[i]=0x00U;
    }
  }
  unsigned char get_size(){
     return index;
  }
  bool get_status(){
     return completed;
  }
  void recognize(unsigned char cb)
{
        switch (index) {
        case 0: // signature
            if (cb!=SIGNATURE) {
                break;
            }
            if (cb==SIGNATURE)   {
                signature = cb;
                ++index;
            break;
            }
        case 1:  {
            dest_id_1 = cb;
            hcrcr ^= cb;
            ++index;
            break;
        }
        case 2:  {
          
            dest_id_2 = cb;
            hcrcr ^= cb;
            ++index;
            break;
        }
        case 3:{ // cmd id or hcrc if mid_command
            cmd = cb;
            hcrcr^= cb;
            ++index;
            break;
        }
        case 0x04: // size
            size = cb;
            hcrcr ^= cb;
            ++index;
            break;

        case 0x05: // hcrc
          hcrc = cb;
            if (hcrc!=hcrcr) { // error in hcrc
                index = 0x00U;
                break;
            }
            if (hcrc==hcrcr) { // zadel for next version oof protocol
              ++index;
            }
            break;

        default: // data+dcrc
            dcrcr ^= cb;
            ++index;
            if (index==size) {
                if (dcrcr!=0x00U) { //bad dcrc
                    //printf("bad dcrc dif 0x%02x\r\n", buf->dcrc);
                  index = 0x00U;
                    break;
                }
                //printf("big cmd %i bytes cmd 0x%02X addr 0x%04X recved\r\n", buf->csize, buf->cid, buf->caddr);
                completed = true;
            }
            break;
        }
  }
};
  
extern RING* uart;
extern RING* i2c;
extern command *CMD;
/***************************************************************************************/
void SystemInit(void);
uint8_t dev_addr(void); /*Функция, определяющая адрес устройства*/
void gpio_config(void);
void i2c_init(void);

#endif