#include"inc.h"
/*******************************************************************************/
void recognize_data(unsigned char data){
  
    if(sig==0xf5U){
    /************************************************/
    if(rxCount==2U){
       dest_id = data;
       crc_head_rec = crc_head_rec ^ data;
    }
    /**********************************************/
    if(rxCount ==3U){
      dest_id +=data;
      crc_head_rec = crc_head_rec ^ data;
    }
    /**********************************************/
    if(rxCount == 4U){
      cmd = data;
       crc_head_rec = crc_head_rec ^ data; 
    }
    /***********************************************/
    if(rxCount == 5U){
      sz=data;
       crc_tail_rec = crc_tail_rec ^data;
    }
    /************************************************/
    if(rxCount == 6U){
      crc_head = data;
    }
    /************************************************/
    if((rxCount > 6U)&&(rxCount < sz+6U)){
      rxData[rxCount - 7U] = data; /*С 7 т.к. адресация идет с нуля*/
      crc_tail_rec = crc_tail_rec ^ data;
    }
    /************************************************/
    if(rxCount==sz+6U){
      crc_tail = data;
    }
    if((crc_head!=0xFFU)&&(rxCount==sz+7U)){
      completed = 1;
      rxCount = 0U;
      completed=1;
    }
    /*************************************************/ 
  }
else if(sig==0xF6U){
  if(rxCount==2){
   offs=temp; 
  }
  else if(rxCount==3){
   size=temp;
  }
  else if(rxCount==4){
   name=temp; 
   completed = 1;
  }
  return;
}
}
uint8_t dev_addr(void){
  uint8_t addr = 0x00U;
  addr = GPIOC->IDR;
  addr|=(0U<<8|0U<<7|1U<<6);
  return addr;
}
void gpio_config(void){
void I2C_recognize();

  GPIO_DeInit(GPIOC);
  GPIO_Init(GPIOC, GPIO_PIN_3, GPIO_MODE_IN_PU_NO_IT);
  GPIO_Init(GPIOC, GPIO_PIN_4, GPIO_MODE_IN_PU_NO_IT);
  GPIO_Init(GPIOC, GPIO_PIN_5, GPIO_MODE_IN_PU_NO_IT);
  GPIO_Init(GPIOC, GPIO_PIN_6, GPIO_MODE_IN_PU_NO_IT);
  GPIO_Init(GPIOC, GPIO_PIN_7, GPIO_MODE_IN_PU_NO_IT);
  GPIO_Init(GPIOD, GPIO_PIN_2, GPIO_MODE_IN_PU_NO_IT);
  GPIO_Init(GPIOD, GPIO_PIN_3, GPIO_MODE_IN_PU_NO_IT);
  return;
}
/*******************************************************************************/
void SystemInit(void)
{
 CLK_DeInit();
 CLK_HSICmd(ENABLE);/*ENABLE INTERNAL RC OSCILLATE*/
 CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);/*PRESCALER 1*/
 CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);/*PRESKALER WITH CPU 1 */ 
 //gpio_config();
 //UART1_DeInit();
 //UART1_Init(9600U, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO, UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);/*UART1 CONFIG*/
 //address = dev_addr();
 I2C_DeInit();
 I2C_Init(100000U, address<<1, I2C_DUTYCYCLE_2,I2C_ACK_CURR, I2C_ADDMODE_7BIT, 16U); /*сдвинуть влево на 1 бит*/
 UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);
 //I2C_ITConfig((I2C_IT_TypeDef)(I2C_IT_ERR | I2C_IT_EVT | I2C_IT_BUF), ENABLE);
 enableInterrupts();
}
#ifdef USE_FULL_ASSERT
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1);
}
#endif

INTERRUPT_HANDLER(UART1_RX_IRQHandler, 18)
{
 temp = UART1->DR;
 ++rxCount;
 UART1_SendData8(address);
 if(temp==0xF5U&&rxCount==1U){
    status=1;
    sig = temp;
    return;
 }
 else if(temp==0xF6U&&rxCount==1U){
    status=1;
    sig=temp;
    return;
 }
 else{
   if(rxCount==1){
      return;
   }
   else{
      status = 1;
      return;
   }
 }
}
INTERRUPT_HANDLER(I2C_IRQHandler, 19)
{
return;
}




  