#ifndef __MAIN_H__
#define __MAIN_H__
#include ".\Library\STC15F.H"

/* #include ".\Hardware\AT24C02.h"
#include ".\Hardware\DHT11.h"
#include ".\Hardware\DS1302.h"
#include ".\Hardware\IIC.h"

#include ".\Hardware\SPI.h"
#include ".\Hardware\SPIOled.h"
#include ".\Hardware\Time0.h"
#include ".\Hardware\Time1.h"
#include ".\Hardware\Uart.h" */
//LED
sbit LED = P4^0;
//Beep
sbit BEEP = P0^4;
//Keyboard
sbit ROW1 = P0^3;
sbit ROW2 = P0^2;
sbit ROW3 = P0^1;
sbit ROW4 = P0^0;
sbit LINE1 = P4^6;
sbit LINE2 = P4^5;
sbit LINE3 = P2^7;
sbit LINE4 = P2^6;
//DTH11
sbit DHT11_DATA = P2^5;
//DS1302
sbit DS1302_SCLK = P2^4;
sbit DS1302_IO = P2^3;
sbit DS1302_CE = P2^2;
//AT24C02  地址111
sbit AT24C02_WP = P2^1;
sbit AT24C02_SCL = P2^0;
sbit AT24C02_SDA = P4^4;
//NRF24L01
sbit NRF24L01_IRQ = P4^3;
sbit NRF24L01_CSN = P4^2;
sbit NRF24L01_SCK = P4^1;
sbit NRF24L01_CE = P3^7;
sbit NRF24L01_MOSI = P1^3;
sbit NRF24L01_MISO = P1^1;
//OLED
//sbit OLED_CS = P1^1;
//sbit OLED_DC = P4^7;
//sbit OLED_RES = P1^2;
//sbit OLED_MOSI = P1^3;
//sbit OLED_CLK = P1^5;
#endif