#ifndef _DHT11_H__
#define _DHT11_H__

void DHTReceiveData(unsigned int DHTDATA[5]);
unsigned char DHTReceiveByte();
void DHT_Display();
unsigned char DHT_Init();

extern unsigned char Time_Update;
#endif