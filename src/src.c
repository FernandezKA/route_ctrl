
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
      uart_tx_data("\n\recieved is completed", 23U);
      rxCount = 0U;
      completed=1;
    }
    /*************************************************/ 
}
/*******************************************************************************/
static void uart_tx_byte(unsigned char data){
  while(!UART1_SR_TXE);
    UART1_DR = data;
    return;
}
static void uart_tx_data(unsigned char * data, unsigned char len){
  while(len--){
    uart_tx_byte(*data++);
  }
}