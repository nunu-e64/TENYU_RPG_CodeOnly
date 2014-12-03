#include "../Define.h"
#include "TrickDamageEffect.h"
#include "Battle.h"


void CTrickDamageEffect_BOMB::DrawDamageEffect(CBattle* _battle, CBImgBank* _bimgbank, CRect _attackerR, CRect _targetR) const{
	//ここに実際のあれこれを書く

	//技発動演出//////////////////////////////////////////////////////////////
	int timecount = 0;
	CVector ball[8];
	int a=60;
	for (int i=0; i<8; i++){
		ball[i].Set(_attackerR.Center().Add(a*cos(i*PI/4),a*sin(i*PI/4)));		
	}
	do{
		_battle->Draw();
		SetDrawBlendMode(DX_BLENDMODE_ADD, timecount*2);
		for (int i=0; i<8; i++){
			DrawExtendGraph(ball[i].x-10, ball[i].y-10, ball[i].x+10, ball[i].y+10, _bimgbank->GetImg("EFFECT_BOMB"), false);
		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		++timecount;
		a-=1;
		for (int i=0; i<8; i++){
			ball[i].Set(_attackerR.Center().Add(a*cos(timecount*PI/(60*2)+i*PI/4),a*sin(timecount*PI/(60*2)+i*PI/4)));
		}
	}while(a!=0 && BasicLoop());

	timecount=0;
	a=20;
	CVector vec = _targetR.Center()-_attackerR.Center();
	vec *= 1/vec.GetLength();
	do{
		_battle->Draw();
		SetDrawBlendMode(DX_BLENDMODE_ADD, 120);
		DrawExtendGraph(ball[0].x-15, ball[0].y-15, ball[0].x+15, ball[0].y+15, _bimgbank->GetImg("EFFECT_BOMB"), false);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		++timecount;
		ball[0].Add(a*vec.x, a*vec.y);
	}while(vec.y*(ball[0].y-_targetR.Center().y)<0 && BasicLoop());
	//////////////////////////////////////////////////////////////////////////

}