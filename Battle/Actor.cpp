#include "../Define.h"
#include "Actor.h"
#include "Battle.h"
#include "../Main/TextBox.h"
//#include "../Main/TextWrap.h"



void CActor::FirstSet(int _playernum, int _enemynum, int _index, CTextBox** _textbox, CCmdList* _cmdlist){
	PLAYER_NUM = _playernum;
	ENEMY_NUM = _enemynum;
	
	ActorIndex = _index;
	Index = ((_index<PLAYER_NUM)? _index: _index-PLAYER_NUM);
	B_TextBox_pp = _textbox;
	CmdList = _cmdlist;

	Alive = Visible = (Hp!=0? true:false);
	SpdPer = between(1.0, 100.0, (double)Spd/100);	//$相対値から絶対値への変換
	OldHp = Hp;

	TimeGauge = 0;//rand()%100;	//ランダムでいいの？$
	Accident = NULL;
	Mode = STAY;
	Target = -1;

	NowTrick = NULL;

	Dx=0; Dy=0;
}

void CActor::SetRect(int _cx, int _cy){
	int width, height;
	GetGraphSize(Img, &width, &height);

	Rect.Left = _cx-width/2;
	Rect.Right = _cx+width/2;
	Rect.Top = _cy-height/2;
	Rect.Bottom = _cy+height/2;
}

/*
void CActor::SetImg(int _img){
	Img = _img;
	SetRect((int)(Rect.Center().x), (int)(Rect.Center().y));
}
*/

bool CActor::SetSystemImg(CBImgBank* _bimgbank){
	Img_hpbar		= _bimgbank->GetImg("HP_BAR");
	Img_timebar[0]	= _bimgbank->GetImg("TIME_BAR1");
	Img_timebar[1]	= _bimgbank->GetImg("TIME_BAR2");
	return true;
}

/*
void CActor::AddTrick(trick_tag const* _trick){
	TrickList.push_back(_trick);
}
*/

bool CActor::Main(){

	if (Alive){
		switch(Mode){
		case PLAN:
		case PREPARE:
			if (TimeGaugeForward()){
				Mode = ACTION;
				return true;
			}
			break;

		case ACTION:
		case STAY:
			if (TimeGaugeForward()){
				Mode = PLAN;
				return true;
			}
			break;
	
		default:
			ErrorDx("Error->CActor::Main->unexpected Mode:%d", __FILE__, __LINE__, Mode);
		}
	}

	return false;
}

bool CActor::Do(){		//行動待機リスト上位のものから行動していく

	if (Alive){
		switch (Mode){
		case PLAN:
			return Plan();
		case ACTION:
			return Action();
		default:
			ErrorDx("Error->CActor::Do->unexpected Mode:%d", __FILE__, __LINE__, Mode);
			return true;
		}
	}else{
		return true;
	}

}

int CActor::Damage(CActor* _attacker, trick_tag const* _trick){
	if (_trick==NULL) {
		ErrorDx("Error->CActor::Damage->_trick==NULL", __FILE__, __LINE__);
		return 0;
	}
	
	int damage  = _trick->Power + _attacker->GetAtk() - Def;	//$ダメージ計算式は要検討
	Hp = between(0, MaxHp, Hp-damage);
	
	return damage;
}

bool CActor::DeadCheck(){	//死亡判定
	
	if (OldHp!=Hp) {
		//Hpバー減少中
		return false;
	}else{
		if (Hp==0) {
			Alive = false;
			char tmp[256];
			sprintf_s(tmp, "%sは倒れた！", GetName().c_str());	//ログウィンドウ作成までのつなぎ$
			(*B_TextBox_pp)->AddStock(tmp);
		}
		return true;
	}
}

void CActor::Draw_Sub(int _dx, int _dy){

	SetDrawBright(255,255,255);
	SetDrawBlendMode( DX_BLENDMODE_NOBLEND , 0 ) ;
	
	//HpBar
	DrawBox(-1+Rect.Center().x-25+_dx, -1+Rect.Bottom+5+_dy, 1+Rect.Center().x+25+_dx, 1+Rect.Bottom+15+_dy, BLUE, true);
	DrawRectGraph((int)(Rect.Center().x-25+_dx), (int)(Rect.Bottom+5+_dy), 0, 0, (int)(50*OldHp/MaxHp), 10, Img_hpbar,false,false);

	//TimeBar
	if (Mode==STAY||Mode==PREPARE) SetDrawBright(150,150,150);
	DrawBox(-1+Rect.Center().x-25+_dx, -1+Rect.Bottom+20+_dy, 1+Rect.Center().x+25+_dx, 1+Rect.Bottom+30+_dy, BLUE, true);
	DrawRectGraph((int)(Rect.Center().x-25+_dx), (int)(Rect.Bottom+20+_dy), 0, 0, (int)(50*TimeGauge/100), 10, ((Mode==STAY||Mode==PLAN)?Img_timebar[0]:Img_timebar[1]),false,false);
	
	//OldHpとHpのギャップを埋める
	if (OldHp>Hp) OldHp--;
	else if (OldHp<Hp) OldHp++;
	
	SetDrawBright(255,255,255);
}



bool CActor::TimeGaugeForward(){
	if (TimeGauge==100)	{
		TimeGauge = 0;
		Mode = (mode_tag)((Mode+1) % MODE_NUM);
	}
	TimeGauge+=SpdPer;
	if (TimeGauge>=100){
		TimeGauge = 100;
		return true;
	}
	return false;
}