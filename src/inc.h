#ifndef _inc_h_
#define _inc_h_
#include "stm8s_conf.h"
/*for uart recieve*/
extern unsigned char rxCount;    /*Количество принятых байтов*/
extern unsigned char rxData[256U];  /*Массив с принятыми данными*/
extern unsigned char txCount;    /*Количество данных для отправки*/
extern unsigned char txData[256U];  /*Массив с данными для отправки*/
extern _Bool status;   /*булевская переменная для указания наличия необработанных приянтых данных*/
extern _Bool completed; /*булевская переменная для указания статуса распознавания данных*/
extern _Bool other_flag;/*будевская переменная для обработки сигнатуры 0xf6*/
extern unsigned char temp;  /*Временная переменная для большинства костылей в этой программе*/
/*for I2C init*/
extern uint8_t address;
/*for sig ==0x5FU*/
extern unsigned char sig;           /*Сигнатура*/
extern unsigned short dest_id;      /*dest id девайса*/
extern unsigned char cmd;           /*Команда в пакете*/
extern unsigned char sz;            /*Размер пакета с данными*/
extern unsigned char crc_head;      /*Crc до байта с размером данных*/
extern unsigned char crc_tail;      /*Crc данных с раздела data*/
extern unsigned char crc_head_rec;
extern unsigned char crc_tail_rec;
/*for sifg == 0xF6U*/
extern unsigned char offs;
extern unsigned char size;
extern unsigned char name;
/***************************************************************************************/
void SystemInit(void);
void recognize_data(unsigned char data);  /*Функция распознавания принятых данных*/
uint8_t dev_addr(void);/*Функция, определяющая адрес устройства*/
void gpio_config(void);
void I2C_recognize();
//void i2c_init(unsigned char addr);

#endif