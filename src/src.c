#include "inc.h"
#include "cmdpacket.h"
/*******************************************************************************/
inline unsigned char dev_addr(void)
{
  volatile unsigned char addr;
  volatile unsigned char temp = GPIOC->IDR;/*read value of gpioc*/
  temp = ~temp;
  addr = ((temp)>>3)|0x30; //MSB
  return addr;
}
void gpio_init(void)
{
  /***********************warning, this part of code is not tested!************/

  GPIO_DeInit(GPIOC);
  GPIO_Init(GPIOC, GPIO_PIN_3, GPIO_MODE_IN_PU_NO_IT); // Пу спасёт нас всех!
  GPIO_Init(GPIOC, GPIO_PIN_4, GPIO_MODE_IN_PU_NO_IT); // Пу спасёт нас всех!
  GPIO_Init(GPIOC, GPIO_PIN_5, GPIO_MODE_IN_PU_NO_IT); // Пу спасёт нас всех!
  GPIO_Init(GPIOC, GPIO_PIN_6, GPIO_MODE_IN_PU_NO_IT); // Пу спасёт нас всех!
  GPIO_Init(GPIOC, GPIO_PIN_7, GPIO_MODE_IN_PU_NO_IT); // Пу спасёт нас всех!
  /************************end part of untested code***************************/
  GPIO_Init(GPIOA, GPIO_PIN_1, GPIO_MODE_OUT_OD_HIZ_SLOW);
  GPIOD->DDR |= 0x18; // bits 4, 3 set as output
  // set up leds on bits 4 and 3
  GPIOD->CR1 |= 0x18; // mode: push-pull
  GPIOD->CR2 |= 0x18; // mode: speed 10 MHz

  return;
}
/*******************************************************************************/
void i2c_init(const unsigned char &addr)
{
  I2C->FREQR = 16U;                //SET 2MHz CLOCKING
  I2C->CR1 |= (1U << 7 | 1U << 0); //DISABLE CLOCK STRECHING AND PERIPH ENABLE
  I2C->CR2 |= (1U << 2);           //ACKNOWLEDGE ENABLE
  I2C->OARL = (addr << 1);         //SET ADDRESS
  I2C->OARH |= (1U << 6);
  I2C->ITR = 0x07; // all I2C interrupt enable
}
/*******************************************************************************/
inline void I2C_transaction_begin(void)
{
  
  if (I2C->SR3 & (1 << 2))    {
    unread = false;
    I2C->DR = lastCmdBuf[0U];
    counter = 1U;
  }
}
inline void I2C_transaction_end(void)/*this function making stop incrementing counter*/
{
  uart->rollback(); 
}
inline void I2C_byte_received(u8 u8_RxData)
{
  i2c->push(u8_RxData);
}
inline u8 I2C_byte_write(void)
{ if(counter==2U){
   lastCmdBuf[0U] = 0U;
}
  return lastCmdBuf[counter++];
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
  UART1->PSCR = 0x01;     // divide the source clock by 1 for IrDA
  //uint8_t address = dev_addr();/*unused variable*/
  I2C_DeInit();
  unsigned char address = dev_addr();
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
INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_BRK_IRQHandler, 11){
  TIM1->SR1&=~TIM1_SR1_UIF;/*CLEAR FLAG*/
  ibuff.getPull();
}
/*******************************************************************************/
INTERRUPT_HANDLER(UART1_RX_IRQHandler, 18) // add led blink here
{

  result = processPacketData(UART1->DR);
  if (result!=0U) unread = true;
  
  GPIOC->ODR ^= 0xF8;

  return;
}
/*******************************************************************************/
INTERRUPT_HANDLER(I2C_IRQHandler, 19)
{
  static u8 sr1;
  static u8 sr2;
  //static u8 sr3;

  // save the I2C registers configuration
  sr1 = I2C->SR1;
  sr2 = I2C->SR2;
  //sr3 = I2C->SR3;

  // Communication error?
  if (sr2 & (I2C_SR2_WUFH | I2C_SR2_OVR | I2C_SR2_ARLO | I2C_SR2_BERR))
  {
    I2C->CR2 |= 255 | I2C_CR2_STOP; // stop communication - release the lines
    I2C->SR2 = 0;                   // clear all error flags
  }
  //More bytes received ?
  if ((sr1 & (I2C_SR1_RXNE | I2C_SR1_BTF)) == (I2C_SR1_RXNE | I2C_SR1_BTF))
  {
    //I2C_byte_received(I2C->DR);
    ibuff.getParse((uint8_t) I2C->DR);
  }
  // Byte received ?
  if (sr1 & I2C_SR1_RXNE)
  {
    //name = I2C->DR;
    //I2C_byte_received(I2C->DR);
    ibuff.getParse((uint8_t) I2C->DR);
  }
  //NAK? (=end of slave transmit comm)
  if (sr2 & I2C_SR2_AF)
  {
    I2C->SR2 &= ~I2C_SR2_AF; // clear AF
    ibuff.endRecieve();
    //I2C_transaction_end();
  }
  // Stop bit from Master  (= end of slave receive comm)
  if (sr1 & I2C_SR1_STOPF)
  {
    I2C->CR2 |= I2C_CR2_ACK; // CR2 write to clear STOPF
    /*at this function we start parsing data*/
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
