
#include "cmdpacket.h"
#include <string.h> // this needed because of memcpy using

unsigned char lastCmdBuf[BUF_SZ]; // last buffered command
unsigned char lastCmdSize=0;

// current receiving command
tCmdBuf buf;

void bufclear(void)
{
    buf.pos = 0;
    buf.csize = 0;
    buf.hcrc = 0xff;
    buf.dcrc = 0;
}

unsigned char *getLastCmd(void) { return lastCmdBuf; }
//inline unsigned char getLastSize(void) { return lastCmdBuf[0U]; }
void setLatch(void)
{
    memcpy(lastCmdBuf+1, buf.data, buf.csize * sizeof(unsigned short));
    lastCmdBuf[0U] = buf.csize;
}

// input: byte, received from uart
// output:  0, if command not done yet
// 			<command size> in lastCmdBuf array, if command has been received after current byte processed

unsigned char processPacketData(unsigned char cb)
{  
    // buf - pointer to instance of command receive buffer
    // size - current input stream size if applicable
    // inbuf - current stream buffer with input data
    //    unsigned char i=0;
    //        printf("byte: 0x%02x\r\n", *pcb);
    buf.data[buf.pos] = cb;
    switch (buf.pos)
    {

    case 0: // signature
        if (cb != SLAVE_SIGNATURE && cb != FRONT_CAR_SIGNATURE && cb != REAR_CAR_SIGNATURE)
        {
            //printf("badsign: 0x%02x\r\n", *pcb);
            // do not needed because already //bufclear(buf);
            break;
        }
        if (cb == SLAVE_SIGNATURE)
            buf.csize = C_STD_PACKET_SZ;
        else if (cb == FRONT_CAR_SIGNATURE)
            buf.csize = C_MID_PACKET_SZ;
        else if (cb == REAR_CAR_SIGNATURE)
            buf.csize = C_SMALL_PACKET_SZ;
        buf.hcrc = C_HCRC_INIT;
        buf.pos += 1;
        break;

    case 1: // addr hi or 2-nd signature
        if (buf.csize == C_SMALL_PACKET_SZ)
        { // small packet arrived ok
            if (buf.data[1] != buf.data[0])
            { // error if signature
                //printf("No 2-nd F6 in short\r\n");
                bufclear();
                break;
            }
            //printf("short cmd\r\n");
            setLatch();
            bufclear();
            return lastCmdBuf[0U];
        }
        buf.hcrc ^= cb;
        buf.pos += 1;
        break;

    case 2: // addr lo
        //buf.caddr = buf->data[1] << 8 | buf->data[2];
        buf.hcrc ^= cb;
        buf.pos += 1;
        break;

    case 3: // cmd id or hcrc if mid_command
        if (buf.csize == C_MID_PACKET_SZ)
        {
            if (buf.hcrc != cb)
            {
                bufclear();
                break;
            }
            //printf("mid cmd @0x%04X\r\n", latch.caddr);
            //buf->cid = 0x80; // pack
            setLatch();
            bufclear();
            return lastCmdBuf[0U];
        }
        buf.hcrc ^= cb;
        buf.pos += 1;
        //buf.cid = buf.data[3];
        break;

    case 4:                                     // size
        buf.csize = C_STD_PACKET_SZ + (cb) + 1; // estimated size, read
        buf.pos += 1;
        buf.hcrc ^= cb;
        break;

    case 5: // hcrc
        if (cb != buf.hcrc)
        { // error in hcrc
            //printf("bad dcrc dif 0x%02x\r\n", cb ^ buf->hcrc);
            bufclear();
            break;
        }
        if (buf.csize == C_STD_PACKET_SZ)
        { // zadel for next version oof protocol
            setLatch();
            bufclear();
            //printf("six-byte command received\r\n");
            return lastCmdBuf[0U];
        }
        buf.pos += 1;
        buf.dcrc = C_DCRC_INIT;
        break;

    default: // data+dcrc
        buf.dcrc ^= cb;
        buf.pos += 1;
        if (buf.pos == buf.csize)
        {
            if (buf.dcrc != 0)
            { //bad dcrc
                //printf("bad dcrc dif 0x%02x\r\n", buf->dcrc);
                bufclear();
                break;
            }
            //printf("big cmd %i bytes cmd 0x%02X addr 0x%04X recved\r\n", buf->csize, buf->cid, buf->caddr);
            setLatch();
            bufclear();
            return lastCmdBuf[0U];
        }
        break;
    } // switch
    return 0;
} // for
