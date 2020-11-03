#include "inc.h"
/*******************************************************************************/
unsigned char MessageBegin;
void recognize_data(unsigned char data)
{
  if (rxCount > 0)
  {
    if (rxCount < 6)
    {
      if (rxCount == 5)
      {
        sz = data;
        put(data, &tbf);
        ++rxCount;
        return;
      }
      else
      {
        crc_head ^= data;
        put(data, &tbf);
        ++rxCount;
        return;
      }
    }
    else
    {
      if (rxCount == sz + 7 && rxCount != 0U)
      {
        crc_tail = data;
        rxCount = 0U;
        if (crc_head == crc_head_rec)
        {
          if (crc_tail == crc_tail_rec)
          {
            GPIOA->ODR |= (1U << 1);
          }
          else
          {
            return;
          }
        }
        else
          return;
      }
    }
  }
  else
  {
    if (data == 0x5F)
    {
      ++rxCount;
      put(data, &tbf);
      return;
    }
    else
    {
      return;
    }
  }
}
void put(unsigned char data, Ring_buff *buff)
{
  buff->buffer[buff->inp++] = data;
  if (buff->inp == 64U)
    buff->inp = 0;
}
unsigned char pop(Ring_buff *buff)
{
  unsigned char ret = buff->buffer[buff->output++];
  if (buff->inp == buff->output)
  {
    buff->inp = 0x00U;
    buff->output = 0x00U;
  }
  return ret;
}
_Bool IsEmpty(Ring_buff *buff)
{
  if ((buff->inp) == (buff->output) || buff->inp == 0U)
    return 1;
  else
    return 0;
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
  I2C->FREQR = 16U;                //SET 2MHz CLOCKING
  I2C->CR1 |= (1U << 7 | 1U << 0); //DISABLE CLOCK STRECHING AND PERIPH ENABLE
  I2C->CR2 |= (1U << 2);           //ACKNOWLEDGE ENABLE
  I2C->OARL = (address << 1);      //SET ADDRESS
  I2C->OARH |= (1U << 6);
  I2C->ITR = 0x07; // all I2C interrupt enable
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
  put(u8_RxData, &bf);
  // return;
}
u8 I2C_byte_write(void)
{
  return txData[txCount++];
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
  UART1_Init(9600U, UART1_WORDLENGTH_8D, UART1_STOPBITS_1, UART1_PARITY_NO, UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_TXRX_ENABLE);
  UART1->CR2 |= (1U << 2);
  UART1->CR2 |= (1U << 3);
  /*UART1 CONFIG*/
  UART1->CR5 |= (1 << 1); // IrDA mode enable
  UART1->PSCR = 0x01;     // divede the source clock by 1 for IrDA
  address = dev_addr();
  I2C_DeInit();
  i2c_init(address);
  //UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);
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
  ++rxCount;
  temp = UART1->DR;
  if (temp == 0x5FU && rxCount == 1U) /*if sign = 0x5f ->write data to variable sig*/
  {
    status = 1; /*enable status*/
    sig = temp; /*write signature to var. sig*/
    return;     /*go out interrupt*/
  }
  else if (rxCount != 0x01U && status)
  {
    rxData[rxCount] = temp; /*add data to array */
  }
  else
  {
    return; /*else ->return out interrupt*/
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
  if (sr2 & (I2C_SR2_WUFH | I2C_SR2_OVR | I2C_SR2_ARLO | I2C_SR2_BERR))
  {
    I2C->CR2 |= I2C_CR2_STOP; // stop communication - release the lines
    I2C->SR2 = 0;             // clear all error flags
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
    I2C->SR2 &= ~I2C_SR2_AF; // clear AF
    I2C_transaction_end();
  }
  // Stop bit from Master  (= end of slave receive comm)
  if (sr1 & I2C_SR1_STOPF)
  {
    I2C->CR2 |= I2C_CR2_ACK; // CR2 write to clear STOPF
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