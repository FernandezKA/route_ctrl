#pragma once

#ifndef _CMDPACKET_H
#define _CMDPACKET_H

#ifdef __cplusplus
extern "C" { 
#endif
  
#define BUF_SZ 65U
#define C_STD_PACKET_SZ 6
#define C_MID_PACKET_SZ 4
#define C_SMALL_PACKET_SZ 2

#define MASTER_SIGNATURE (0xf5U)
#define SLAVE_SIGNATURE (0x5fU)
//#define REAR_CAR_SIGNATURE (0xf6U)
#define FRONT_CAR_SIGNATURE (0x54U)
#define C_HCRC_INIT (0xffU)
#define C_FRONT_HCRC_INIT (0x33U)
#define C_DCRC_INIT (0x00U)

typedef struct
{
  unsigned char pos;   // buffer current position and receive fsm
  unsigned char csize; // command size, if applicable
  unsigned char hcrc;
  unsigned char dcrc;
  unsigned char data[BUF_SZ];
} tCmdBuf;

extern unsigned char lastCmdBuf[BUF_SZ]; // last buffered command

void bufclear(void);
unsigned char *getLastCmd(void);
void setLatch(void);
unsigned char processPacketData(unsigned char cb);


// example of using
/*
unsigned char cmdLen=0;
unsigned char uart_b = UART->DR;
cmdLen = processPacketData(uart_b);
if (cmdLen!=0) { // put data in i2c buffer and turn GPIO flag active (down)
}
*/

#ifdef __cplusplus
} 
#endif

#endif