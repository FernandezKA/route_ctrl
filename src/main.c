#include "stm8s_conf.h"
#include"inc.h"
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
return;
}
/*INTERRUPT_HANDLER(I2C_IRQHandler, 20){
  I2C_SendData(0xFFU);
  return;
}  */
  