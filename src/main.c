
#include"inc.h"
/*************************/
/*for uart_recieve*/
unsigned char rxCount=0U;    /*Количество принятых байтов*/
unsigned char rxData[256U];  /*Массив с принятыми данными*/
unsigned char txCount=0U;    /*Количество данных для отправки*/
unsigned char txData[256U];  /*Массив с данными для отправки*/
_Bool status = 0;   /*булевская переменная для указания наличия необработанных приянтых данных*/
_Bool completed = 0; /*булевская переменная для указания статуса распознавания данных*/
_Bool other_flag = 0;/*будевская переменная для обработки сигнатуры 0xf6*/
unsigned char temp;  /*Временная переменная для большинства костылей в этой программе*/
/*for I2C init*/
uint8_t address = 0xFFU;/*default value*/
/*sig == 0xF5U*/
unsigned char sig=0xFFU;           /*Сигнатура*/
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
unsigned char name;
/*************************/
int main(void)
{
  SystemInit();
	while (1){
      if((status)&&(!completed)){/*Если байт на прерывании принят, выполняем функцию распознавания*/
   status = 0;/*Опускаем байт необработанной информации*/
   recognize_data(temp);
  }
      if(completed){
     completed=0;
    }
  };
}

