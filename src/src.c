#include"inc.h"
/*******************************************************************************/
void recognize_data(unsigned char data){
  
    if(rxCount  == 1U){
      if (data == 0xf5){
        sig = data;
      }
      else if (data == 0xf6){
         sig = data;
         other_flag = 1;
      }
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
uint8_t dev_addr(void){
  
   return 0xFFU;
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
#ifdef USE_FULL_ASSERT
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
  