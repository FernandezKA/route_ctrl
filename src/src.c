#include "inc.h"
/*******************************************************************************/
unsigned char cnt = 0U;
unsigned char last = 0xFFU;
unsigned char tmp[] = {0x5FU, 0x00U, 0x01U, 0x80U, 0x00U, 0x7EU};
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
  I2C->FREQR |= (1U << 1);         //SET 2MHz CLOCKING
  I2C->CR1 |= (1U << 7 | 1U << 0); //DISABLE CLOCK STRECHING AND PERIPH ENABLE
  I2C->CR2 |= (1U << 2);           //ACKNOWLEDGE ENABLE
  I2C->OARL = (address << 1);      //SET ADDRESS
  //I2C->ITR |= (1U << 2 | 1U << 1); //ENABLE ITEVTEN AND ITBUFEN
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
  i2c_init(0x6eU);
  //I2C_Init(10000U, 0x32U<<1, I2C_DUTYCYCLE_2,I2C_ACK_CURR, I2C_ADDMODE_7BIT, 16U); /*сдвинуть влево на 1 бит*/
  UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);
  I2C_ITConfig((I2C_IT_TypeDef)(I2C_IT_EVT|I2C_IT_BUF), ENABLE);//
  //enableInterrupts();
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
  asm("SIM");
  volatile unsigned char sr1;
  volatile unsigned char sr2;
  volatile unsigned char sr3;
  sr1 = I2C->SR1;
  sr2=I2C->SR2;
  sr3 = I2C->SR3;
  if (rxCount < 255)
  {
    rxData[rxCount++] = sr1;
    rxData[rxCount++] = sr2;
    rxData[rxCount++] = sr3;
  }

  if (BitMask(sr1, 1U << 1)) //addr rec
  {
    sr2 = I2C->SR1;
    sr2 = I2C->SR3;
  
    //i2cst = 1; //enable TR status
  }

  if (BitMask(sr1, 1U << 4))//rec stop-bit
  {
    sr2 = I2C->SR1;
    I2C->CR2 = (1U << 2);
  }
   if (BitMask(sr1, 1U << 7))//if TXE==1, send data to DR
  {
    I2C->DR = 0x5fU;
  }
  if (BitMask(sr1, 1U << 6))//if RXNE==1, recieve data from DR to variable
  {
    name = I2C->DR;
  }
  /*

  if (i2cst)
  {
    if (BitMask(sr3, 1U << 2)) //if TRA is 1, data byte is transmitted
    {
      I2C_SendData(rxData[rxCount--]);
    }
    else //else recieved byte
    {
      rxData[rxCount++] = I2C_ReceiveData(); //push to array with incremental index
    }
  }
  if(BitMask(sr1, 1U<<0)){
    sr2 = I2C->SR3;
    I2C->CR2|=1U<<1;//write to CR2;
    i2cst=0;
  } */
  if(I2C->SR2!=0x00U){//if exist errors, write 0x00U
    I2C->SR2 = 0x00U;
  }
  asm("RIM");
  return;
}
