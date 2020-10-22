
#include"inc.h"
/*************************/
unsigned char rxCount=0U;    /*Количество принятых байтов*/
unsigned char rxData[256U];  /*Массив с принятыми данными*/
unsigned char txCount=0U;    /*Количество данных для отправки*/
unsigned char txData[256U];  /*Массив с данными для отправки*/
unsigned char sig;           /*Сигнатура*/
unsigned short dest_id;      /*dest id девайса*/
unsigned char cmd;           /*Команда в пакете*/
unsigned char sz;            /*Размер пакета с данными*/
unsigned char crc_head;      /*Crc до байта с размером данных*/
unsigned char crc_tail;      /*Crc данных с раздела data*/
unsigned char crc_head_rec = 0xFFU;
unsigned char crc_tail_rec = 0xFFU;
_Bool status = 0;   /*булевская переменная для указания наличия необработанных приянтых данных*/
_Bool completed = 0; /*булевская переменная для указания статуса распознавания данных*/
_Bool other_flag = 0;/*будевская переменная для обработки сигнатуры 0xf6*/
unsigned char temp;  /*Временная переменная для большинства костылей в этой программе*/
/*************************/
int main(void)
{
  SystemInit();
	while (1){
  UART1_SendData8(0xFFU);
      if((status)&&(!completed)){/*Если байт на прерывании принят, выполняем функцию распознавания*/
   status = 0;/*Опускаем байт необработанной информации*/
   recognize_data(temp);
  }
      if(completed){
     completed=0;
    }
  };
}

