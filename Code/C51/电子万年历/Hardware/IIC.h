#ifndef __IIC_H__
#define __IIC_H__

void IIC_Init();
void IIC_Start(void);
void IIC_Stop(void);
void IIC_Ack(void);
void IIC_Not_Ack(void);
unsigned char IIC_ReceiveAck();
void IIC_SendByte(unsigned char SendByte);
unsigned char IIC_ReadByte(unsigned char Ack);// Ack为1表示应答0表示不应答
#endif


