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
extern RING* uart;
extern RING* i2c;
extern RING* tmp;
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
  unsigned char data[8U];
  unsigned char index;
  unsigned char size_of_packet;
  bool completed;
public:
  command (){
     index = 0;
     hcrcr = dcrcr = 0xFFU;
     buf_clear();
  }
  ~command(){
     asm("nop");
  }
  void buf_clear() {
    for(uint8_t i = 0; i<8U; ++i){
      data[i]=0x00U;
    }
  }
  void clean_all(){
 signature =
 dest_id_1=
 dest_id_2 =
 cmd       =
 size      =
 hcrc      =
 dcrc      =
 hcrcr     =
dcrcr      =
index      =
size_of_packet = 0U;
  }
  unsigned char get_size(){
     int temp = size_of_packet;
     size_of_packet = 0;
     return temp;
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
                completed = false;
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
            if (hcrc==hcrcr) {
              if(size==0U){
                 size_of_packet = index + 1U;
                 index = 0x00U;
                 completed = true;
                 uart->push(signature);
                 uart->push(dest_id_1);
                 uart->push(dest_id_2);
                 uart->push(cmd);
                 uart->push(size);
                 uart->push(hcrcr);
                 break;
              }
              ++index;
            }
            break;

        default: // data+dcrc
          if(index == size + 6U){      /*if data is tcrc*/
             dcrc = cb;
             if(dcrcr = dcrc){
                  size_of_packet = index;
                  index = 0x00U;
                  completed = true;
                  uart->push(signature);
                  uart->push(dest_id_1);
                  uart->push(dest_id_2);
                  uart->push(cmd);
                  uart->push(size);
                  uart->push(hcrc);
                  while(tmp ->count!=0U){
                     uart->push(tmp->pull());
                  }
                  uart->push(dcrc);
                  clean_all();
                    break;
             }
          }
          else{
            dcrcr ^= cb;
            tmp->push(cb);
            ++index;
            break;
        }
    }
  }
};
  
    extern command* CMD;
/***************************************************************************************/
void SystemInit(void);
uint8_t dev_addr(void); /*Функция, определяющая адрес устройства*/
void gpio_config(void);
void i2c_init(void);

#endif