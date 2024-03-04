#ifndef __DHT11_H
#define __DHT11_H


void DHT11_receive();      //接收40位的数据
extern unsigned char rec_dat[9], T_H; //储存数据
#endif
