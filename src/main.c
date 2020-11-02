#include"inc.h"
/*******************************************************************************/
/*for uart_recieve*/
unsigned char rxCount=0x00U;    /*Количество принятых байтов*/
unsigned char rxData[64U];  /*Массив с принятыми данными*/
unsigned char txCount=0x00U;    /*Количество данных для отправки*/
unsigned char txData[64U];  /*Массив с данными для отправки*/
/*******************************************************************************/
_Bool status = 0;   /*булевская переменная для указания наличия необработанных приянтых данных*/
_Bool completed = 0; /*булевская переменная для указания статуса распознавания данных*/
_Bool resData = 0;/*будевская переменная для обработки сигнатуры 0xf6*/
unsigned char temp;  /*Временная переменная для большинства костылей в этой программе*/
extern unsigned char cnt = 0x00U;
/*for I2C init*/
uint8_t address = 0x6eU;/*default value*/
/*sig == 0xF5U*/
unsigned char sig;         /*Сигнатура*/
unsigned short dest_id;      /*dest id девайса*/
unsigned char cmd;           /*Команда в пакете*/
unsigned char sz;            /*Размер пакета с данными*/
unsigned char crc_head;      /*Crc до байта с размером данных*/
unsigned char crc_tail;      /*Crc данных с раздела data*/
unsigned char crc_head_rec = 0xFFU;
unsigned char crc_tail_rec = 0xFFU;
/*sig=0xF6U*/
unsigned char offs ;
unsigned char size;
unsigned char name = 0x00U;
Ring_buff bf;
/*******************************************************************************/
int main(void)
{
  SystemInit();
  bf.inp=0x00U;
  bf.output=0x00U;
while(1){
  while(bf.inp!=bf.output){
    if(BitMask(UART1->SR, 1U<<7)){/*waiting while txe==1*/
    UART1->DR=pop(&bf);     /*send data to DR of uart*/
      }
    }
  
  }
}
