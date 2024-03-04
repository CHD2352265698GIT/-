#ifndef __MUSIC_H
#define __MUSIC_H
#include "stdint.h"              
#define playerrortime 10
//ÓïÒôÄ£¿é¿ØÖÆ
enum{
	null,
	play,
	stop,
	next,
	num,
	volume
};

extern char mu_play[5];
extern char mu_stop[5];
extern char mu_next[5];
extern char mu_select[7];
extern char mu_volume[6];

void MusicControl(char command,char data);
void HAL_Delay_us(uint32_t us);


#endif
