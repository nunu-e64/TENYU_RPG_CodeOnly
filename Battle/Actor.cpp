#include "../Define.h"
#include "Actor.h"
#include "Battle.h"



void CActor::FirstSet(int _index, CTextBox** _textbox, CCmdList* _cmdlist, std::map <int,int> *_imgbank){
	ActorIndex = _index;
	Index = ((_index<MAX_PLAYER)? _index: _index-MAX_PLAYER);
	B_TextBox_pp = _textbox; 
	CmdList = _cmdlist;
	Img_hpbar = (*_imgbank)[CBattle::HP_BAR];
	Img_timebar[0] = (*_imgbank)[CBattle::TIME_BAR1];
	Img_timebar[1] = (*_imgbank)[CBattle::TIME_BAR2];
}

void CActor::SetValue(int _atk, int _def, double _spd, int _maxhp){
	Alive = Visible = true;

	Atk = max(1,_atk);
	Def = max(1,_def);
	Spd = between(1.0,100.0,_spd);
	Hp = OldHp = MaxHp = _maxhp;

	TimeGauge = 0;//rand()%100;	//ランダムでいいの？$
	Accident = NULL;
	Mode = STAY;
	Target = -1;

	NowTrick = NULL;

	Init();
}

void CActor::SetRect(int _cx, int _cy){
	int width, height;
	GetGraphSize(Img, &width, &height);

	Rect.Left = _cx-width/2;
	Rect.Right = _cx+width/2;
	Rect.Top = _cy-height/2;
	Rect.Bottom = _cy+height/2;
}

void CActor::SetImg(int _img){
	Img = _img;
	SetRect(Rect.Center().x, Rect.Center().y);
}

void CActor::AddTrick(trick_tag const* _trick){
	TrickList.push_back(_trick);
}

bool CActor::Main(){

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

	return false;
}

bool CActor::Do(){		//行動待機リスト上位のものから行動していく
	switch (Mode){
	case PLAN:
		if (Plan()){
			return true;
		}else{
			return false;
		}
	case ACTION:
		if (Action()){
			return true;
		}else{
			return false;
		}
	default:
		ErrorDx("Error->CActor::Do->unexpected Mode:%d", __FILE__, __LINE__, Mode);
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

bool CActor::DeadCheck(){
	
	if (OldHp!=Hp) {
		return false;
	}else{
		if (Hp==0) Alive = false;
		return true;
	}
}


bool CActor::TimeGaugeForward(){
	if (TimeGauge==100)	{
		TimeGauge = 0;
		Mode = (mode_tag)((Mode+1) % MODE_NUM);
	}
	TimeGauge+=Spd;
	if (TimeGauge>=100){
		TimeGauge = 100;
		return true;
	}
	return false;
}
//
//void CActor::DamageStep(int _targetnum, ...){
//	va_list args;
//	va_start( args, _targetnum );
//	
//	for (int i=0; i<_targetnum; i++){
//		DamageStep(va_arg(args, CActor*));
//	}
//
//	va_end( args );
//}
//
//void CActor::DamageStep(CActor* _target){
//	int damage=0;
//
//	if (strlen(NowTrick->Name)){
//		damage = NowTrick->Power + Atk - _target->GetDef();
//	}
//	damage = max(1,damage);
//
//	_target->Damage(damage);
//	//ダメージメッセージ？
//}
