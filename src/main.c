#include "stm8s_conf.h"
/**
  *inc->stm8s_conf.h - раскоментировать строчки с нужными хедерами 
  *периферии и необходимыми обработчиками прерываний. Все закоментированные
  *обработчики ведут на бесконечные циклы.
  * 
  *Project->Options->General Options - выбрать мк
  *
  *Project->Options->Debugger - выбрать отладчик
  *
  *Project->Options->C/C++ Compiler->Preprocessor->Defined symbols  - задать
  *семейство процессора(перечислены в lib->SPL->inc->stm8s.h), а также задать
  *частоты внутренних и внешних генераторов(если не задать, то будут ипользованы
  *значения по умолчанию из stm8s.h).
  */
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
unsigned char temp;  /*Временная переменная для большинства костылей в этой программе*/
/*************************************************************************************/
static void recognize_data(unsigned char data);  /*Функция распознавания принятых данных*/

/*******************************************************************************/
static void recognize_data(unsigned char data){
  
    if(rxCount  == 1U){
       sig = data;
       /*uart_tx_data("\n\rSig recognized : ",19U);
       uart_tx_byte(sig); */
    }
    /************************************************/
    if(rxCount==2U){
       dest_id = data;
       crc_head_rec = crc_head_rec ^ data;
    }
    /**********************************************/
    if(rxCount ==3U){
      dest_id +=data;
      crc_head_rec = crc_head_rec ^ data;
      /*uart_tx_data("\n\rDest_id recognized ", 22U);*/
    }
    /**********************************************/
    if(rxCount == 4U){
      cmd = data;
       crc_head_rec = crc_head_rec ^ data;
       /*uart_tx_data("\n\rCommand is :", 14U);
       uart_tx_byte(cmd); */ 
    }
    /***********************************************/
    if(rxCount == 5U){
      sz=data;
       crc_tail_rec = crc_tail_rec ^data;
       /*uart_tx_data("\n\rSize of packet: ", 18U);
       uart_tx_byte(sz);*/
    }
    /************************************************/
    if(rxCount == 6U){
      crc_head = data;
     /* uart_tx_data("\r\nCrc_head: ", 11U);
      uart_tx_byte(crc_head);
      uart_tx_data("\r\nCRC_head recognized: ", 22U);
      uart_tx_byte(crc_head);  */
      
    }
    /************************************************/
    if((rxCount > 6U)&&(rxCount < sz+6U)){
      rxData[rxCount - 7U] = data; /*С 7 т.к. адресация идет с нуля*/
      crc_tail_rec = crc_tail_rec ^ data;
     /* uart_tx_data("\r\n", 2U);
      uart_tx_byte(rxData[rxCount-7U]); */ 
    }
    /************************************************/
    if(rxCount==sz+6U){
      crc_tail = data;
     /* uart_tx_data("array of data", 13U);
      uart_tx_data(rxData, sz); */
    }
    if((crc_head!=0xFFU)&&(rxCount==sz+7U)){
      rxCount = 0U;
      completed=1;
    }
    /*************************************************/ 
}
/*******************************************************************************/
int SystemInit(void)
{
 CLK_DeInit();
 CLK_HSICmd(ENABLE);/*ENABLE INTERNAL RC OSCILLATE*/
 CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);/*PRESCALER 1*/
 CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);/*PRESKALER WITH CPU 1 */
 UART1_DeInit();
 UART1_Init(9600U, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO, UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);/*UART1 CONFIG*/
 I2C_DeInit();
 I2C_Init(400000U, 0xFFU, I2C_DUTYCYCLE_2,I2C_ACK_CURR, I2C_ADDMODE_7BIT, 16U);
 UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);
 I2C_ITConfig(I2C_IT_EVT, ENABLE);
 I2C_ITConfig(I2C_IT_BUF, ENABLE);
 enableInterrupts();
    return 0;
}

void main(void)
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

  #ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
    I2C_SendData(0xFFU);
  }
}
#endif

INTERRUPT_HANDLER(UART1_RX_IRQHandler, 18)
{
 temp = UART1->DR;
 ++rxCount;
 status = 1; 
 UART1->DR=temp;
return;
}
/*INTERRUPT_HANDLER(I2C_IRQHandler, 20){
  I2C_SendData(0xFFU);
  return;
}  */
  