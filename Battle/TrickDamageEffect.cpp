#include "../Define.h"
#include "TrickDamageEffect.h"
#include "Battle.h"


void CTrickDamageEffect_BOMB::DrawDamageEffect(CBattle* _battle, CBImgBank* _bimgbank, CRect _attackerR, CRect _targetR) const{
	//ここに実際のあれこれを書く


}

void CTrickDamageEffect_PROTO::DrawDamageEffect(CBattle* _battle, CBImgBank* _bimgbank, CRect _attackerR, CRect _targetR) const{
	//ここに実際のあれこれを書く

	//技発動演出//////////////////////////////////////////////////////////////
	int timecount = 0;
	CVector ball[8];
	int a=Radius;
	for (int i=0; i<8; i++){
		ball[i].Set(_attackerR.Center().Add(a*cos(i*PI/4),a*sin(i*PI/4)));		
	}
	do{
		_battle->Draw();
		SetDrawBlendMode(DX_BLENDMODE_ADD, timecount*2);
		for (int i=0; i<8; i++){
			DrawExtendGraph(ball[i].x-Size1, ball[i].y-Size1, ball[i].x+Size1, ball[i].y+Size1, _bimgbank->GetImg("EFFECT_BOMB"), false);
		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		++timecount;
		a-=1;
		for (int i=0; i<8; i++){
			ball[i].Set(_attackerR.Center().Add(a*cos(timecount*PI/(Radius*2)+i*PI/4),a*sin(timecount*PI/(Radius*2)+i*PI/4)));
		}
	}while(a!=0 && BasicLoop());

	timecount=0;
	a=20;	//光球の移動速度
	CVector vec = _targetR.Center()-_attackerR.Center();
	vec *= 1/vec.GetLength();
	do{
		_battle->Draw();
		SetDrawBlendMode(DX_BLENDMODE_ADD, 120);
		DrawExtendGraph(ball[0].x-Size2, ball[0].y-Size2, ball[0].x+Size2, ball[0].y+Size2, _bimgbank->GetImg("EFFECT_BOMB"), false);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		++timecount;
		ball[0].Add(a*vec.x, a*vec.y);
	}while( BasicLoop() && vec.y*(ball[0].y-_targetR.Center().y)<0);
	//////////////////////////////////////////////////////////////////////////

}