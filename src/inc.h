#ifndef _inc_h_
#define _inc_h_
#include "stm8s_conf.h"
#define BitMask(a, b) (((a) & (b)) == (b))
/*for uart recieve*/
extern unsigned char rxCount;	  /*Количество принятых байтов*/
extern unsigned char rxData[64U]; /*Массив с принятыми данными*/
extern unsigned char txCount;	  /*Количество данных для отправки*/
extern unsigned char txData[64U]; /*Массив с данными для отправки*/
extern _Bool status;			  /*булевская переменная для указания наличия необработанных приянтых данных*/
extern _Bool completed;			  /*булевская переменная для указания статуса распознавания данных*/
extern _Bool resData;
extern unsigned char temp;
extern unsigned char cnt;
/*for I2C init*/
extern uint8_t address;
/*for sig ==0x5FU*/
extern unsigned char sig;	   /*Сигнатура*/
extern unsigned char dest_id; /*dest id девайса*/
extern unsigned char dest_id_1;
extern unsigned char cmd;	   /*Команда в пакете*/
extern unsigned char sz;	   /*Размер пакета с данными*/
extern unsigned char crc_head; /*Crc до байта с размером данных*/
extern unsigned char crc_tail; /*Crc данных с раздела data*/
extern unsigned char crc_head_rec;
extern unsigned char crc_tail_rec;
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
extern Ring_buff tbf;
/***************************************************************************************/
void SystemInit(void);
void recognize_data(unsigned char data); /*Функция распознавания принятых данных*/
uint8_t dev_addr(void);										  /*Функция, определяющая адрес устройства*/
void gpio_config(void);
void I2C_recognize();
void i2c_init(unsigned char addr);
void I2C_transaction_begin(void);

#endif