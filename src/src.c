#include "inc.h"
/*******************************************************************************/
unsigned char MessageBegin;
void recognize_data(unsigned char data)
{

  if (sig == 0xf5U)
  {
    /************************************************/
    if (rxCount == 2U)
    {
      dest_id = data;
      crc_head_rec = crc_head_rec ^ data;
    }
    /**********************************************/
    if (rxCount == 3U)
    {
      dest_id += data;
      crc_head_rec = crc_head_rec ^ data;
    }
    /**********************************************/
    if (rxCount == 4U)
    {
      cmd = data;
      crc_head_rec = crc_head_rec ^ data;
    }
    /***********************************************/
    if (rxCount == 5U)
    {
      sz = data;
      crc_tail_rec = crc_tail_rec ^ data;
    }
    /************************************************/
    if (rxCount == 6U)
    {
      crc_head = data;
    }
    /************************************************/
    if ((rxCount > 6U) && (rxCount < sz + 6U))
    {
      rxData[rxCount - 7U] = data; /*С 7 т.к. адресация идет с нуля*/
      crc_tail_rec = crc_tail_rec ^ data;
    }
    /************************************************/
    if (rxCount == sz + 6U)
    {
      crc_tail = data;
    }
    if ((crc_head != 0xFFU) && (rxCount == sz + 7U))
    {
      completed = 1;
      rxCount = 0U;
      completed = 1;
    }
    /*************************************************/
  }
  else if (sig == 0xF6U)
  {
    if (rxCount == 2)
    {
      offs = temp;
    }
    else if (rxCount == 3)
    {
      size = temp;
    }
    else if (rxCount == 4)
    {
      name = temp;
      completed = 1;
    }
    return;
  }
}
void put(unsigned char data, Ring_buff* buff){
  buff->buffer[buff->inp++]=data;
  if(buff->inp == 64U) buff->inp=0;
}
unsigned char pop(Ring_buff* buff){
  if(!IsEmpty) return 0;
  unsigned char ret = buff->buffer[buff->output++];
  if(buff->output==64U) buff->output = 0U;
  return ret;
}
_Bool IsEmpty(Ring_buff* buff){
  if((buff->inp)==(buff->output)||buff->inp ==0U) return 1;
  else return 0;
}
unsigned char dev_addr(void)
{
  volatile unsigned char addr;
  addr = GPIOC->IDR;
  addr &= (0x3FU); //побитовое и для зануления 7 и 6 битов
  addr |= (1U << 5);
  return 0x32U;
}
void gpio_init(void)
{
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
void i2c_init(unsigned char addr)
{
  I2C->FREQR = 16U;         //SET 2MHz CLOCKING
  I2C->CR1 |= (1U << 7 | 1U << 0); //DISABLE CLOCK STRECHING AND PERIPH ENABLE
  I2C->CR2 |= (1U << 2);           //ACKNOWLEDGE ENABLE
  I2C->OARL = (address << 1);      //SET ADDRESS
  I2C->OARH|=(1U<<6);
  I2C->ITR	= 0x07;					      // all I2C interrupt enable
}
/*******************************************************************************/
	void I2C_transaction_begin(void)
	{
		MessageBegin = TRUE;
	}
	void I2C_transaction_end(void)
	{
		//Not used in this example
	}
	void I2C_byte_received(u8 u8_RxData)
	{
           //put(u8_RxData, &bf);
          put(u8_RxData, &bf);
          return;
		}
	u8 I2C_byte_write(void)
	{
		/*if (u8_MyBuffp < &u8_My_Buffer[MAX_BUFFER])
			return *(u8_MyBuffp++);
		else
			return 0x00;*/
      return pop(&bf);
	}
/*******************************************************************************/
void SystemInit(void)
{
  CLK_DeInit();
  CLK_HSICmd(ENABLE);                            /*ENABLE INTERNAL RC OSCILLATE*/
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); /*PRESCALER 1*/
  CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);       /*PRESKALER WITH CPU 1 */
  gpio_init();
  UART1_DeInit();
  UART1_Init(9600U, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO, UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE); /*UART1 CONFIG*/
  address = dev_addr();
  I2C_DeInit();
  i2c_init(address);
  UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);
  enableInterrupts();
}
/*******************************************************************************/
#ifdef USE_FULL_ASSERT
void assert_failed(u8 *file, u32 line)
{
  while (1)
    ;
}
#endif
/*******************************************************************************/
INTERRUPT_HANDLER(UART1_RX_IRQHandler, 18)
{
  temp = UART1->DR;
  UART1->DR = temp;
  ++rxCount;
  UART1_SendData8(address);
  if (temp == 0xF5U && rxCount == 1U)
  {
    status = 1;
    sig = temp;
    return;
  }
  else if (temp == 0xF6U && rxCount == 1U)
  {
    status = 1;
    sig = temp;
    return;
  }
  else
  {
    if (rxCount == 1)
    {
      return;
    }
    else
    {
      status = 1;
      return;
    }
  }
}
/*******************************************************************************/
INTERRUPT_HANDLER(I2C_IRQHandler, 19)
{
	static u8 sr1;					
	static u8 sr2;
	static u8 sr3;
	
// save the I2C registers configuration
sr1 = I2C->SR1;
sr2 = I2C->SR2;
sr3 = I2C->SR3;

// Communication error? 
  if (sr2 & (I2C_SR2_WUFH | I2C_SR2_OVR |I2C_SR2_ARLO |I2C_SR2_BERR))
  {		
    I2C->CR2|= I2C_CR2_STOP;  // stop communication - release the lines
    I2C->SR2= 0;					    // clear all error flags
	}
//More bytes received ? 
  if ((sr1 & (I2C_SR1_RXNE | I2C_SR1_BTF)) == (I2C_SR1_RXNE | I2C_SR1_BTF))
  {
    I2C_byte_received(I2C->DR);
  }
// Byte received ? 
  if (sr1 & I2C_SR1_RXNE)
  {
    //name = I2C->DR;
    I2C_byte_received(I2C->DR);
  }
 //NAK? (=end of slave transmit comm) 
  if (sr2 & I2C_SR2_AF)
  {	
    I2C->SR2 &= ~I2C_SR2_AF;	  // clear AF
		I2C_transaction_end();
	}
// Stop bit from Master  (= end of slave receive comm)
  if (sr1 & I2C_SR1_STOPF) 
  {
    I2C->CR2 |= I2C_CR2_ACK;	  // CR2 write to clear STOPF
		I2C_transaction_end();
	}
// Slave address matched (= Start Comm) 
  if (sr1 & I2C_SR1_ADDR)
  {	 
		I2C_transaction_begin();
	}
// More bytes to transmit ? 
  if ((sr1 & (I2C_SR1_TXE | I2C_SR1_BTF)) == (I2C_SR1_TXE | I2C_SR1_BTF))
  {
		I2C->DR = I2C_byte_write();
  }
// Byte to transmit ? 
  if (sr1 & I2C_SR1_TXE)
  {
    //I2C->DR = name;
    I2C->DR = I2C_byte_write();
  }	
}