#ifndef _inc_h_
#define _inc_h_
#include "stm8s_conf.h"
extern unsigned char rxCount;    /*Количество принятых байтов*/
extern unsigned char rxData[256U];  /*Массив с принятыми данными*/
extern unsigned char txCount;    /*Количество данных для отправки*/
extern unsigned char txData[256U];  /*Массив с данными для отправки*/
extern unsigned char sig;           /*Сигнатура*/
extern unsigned short dest_id;      /*dest id девайса*/
extern unsigned char cmd;           /*Команда в пакете*/
extern unsigned char sz;            /*Размер пакета с данными*/
extern unsigned char crc_head;      /*Crc до байта с размером данных*/
extern unsigned char crc_tail;      /*Crc данных с раздела data*/
extern unsigned char crc_head_rec;
extern unsigned char crc_tail_rec;
extern _Bool status;   /*булевская переменная для указания наличия необработанных приянтых данных*/
extern _Bool completed; /*булевская переменная для указания статуса распознавания данных*/
extern _Bool other_flag;/*будевская переменная для обработки сигнатуры 0xf6*/
extern unsigned char temp;  /*Временная переменная для большинства костылей в этой программе*/
/***************************************************************************************/
int SystemInit(void);
void recognize_data(unsigned char data);  /*Функция распознавания принятых данных*/
uint8_t dev_addr(void);/*Функция, определяющая адрес устройства*/
#endif