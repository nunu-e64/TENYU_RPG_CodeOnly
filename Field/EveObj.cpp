#include "../Define.h"
#include "EveObj.h"

void CEveObj::Draw(int _x, int _y){
	
	if (Visible){
		//static int a=200;	//使ってないっぽいのでコメントアウト。なんじゃこれ。14/08/31

		switch(Effect){
		case NONE:
			break;
		case BLINK:
			Alpha = between(0,255,Alpha+EffectNum[3]);
			if (Alpha<=(EffectNum[0]*255/100)) EffectNum[3] = -EffectNum[3];
			if (Alpha>=(EffectNum[1]*255/100)) EffectNum[3] = -EffectNum[3];
			Alpha = between(EffectNum[0]*255/100, EffectNum[1]*255/100, (int)Alpha);
			break;

		case RND_DIR:
			if (++EffectNum[1]==EffectNum[0]){
				EffectNum[1] = 0;
				Dir = (direction_tag) mod(Dir+1, DIRECTION_NUM);
			}
			break;

		case WALK:
			break;
		}
		SetDrawBlendMode( DX_BLENDMODE_ALPHA, Alpha);
		if (Kind==NPC){
			DrawGraph(_x+Dx%MAP_CHIP_SIZE, _y+Dy%MAP_CHIP_SIZE, Img[Dir*4+mod(Step,4)],true);
		}else{
			DrawGraph(_x+Dx%MAP_CHIP_SIZE, _y+Dy%MAP_CHIP_SIZE, Img[0],true);
		}
		SetDrawBlendMode( DX_BLENDMODE_NOBLEND , 0 );
	}

}