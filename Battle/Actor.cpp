#include "../Define.h"
#include "Actor.h"
#include "BImgBank.h"
#include "../Main/TextBox.h"
#include "LogWindow.h"

void CActor::FirstSet(int _playernum, int _enemynum, int _index, CTextBox** _textbox, CCmdList* _cmdlist, CLogWindow* _logWindow){
	PLAYER_NUM = _playernum;
	ENEMY_NUM = _enemynum;
	
	ActorIndex = _index;
	Index = ((_index<PLAYER_NUM)? _index: _index-PLAYER_NUM);
	B_TextBox_pp = _textbox;
	LogWindow = _logWindow;
	CmdList = _cmdlist;

	Alive = Visible = (Hp!=0? true:false);
	VisibleStatus = (Alive? VISIBLE:INVISIBLE);
	SpdPer = between(0.001, 100.0, (double)Spd);	//$相対値から絶対値への変換
	OldHp = Hp;

	MaxTimeGauge = 100;	//%
	TimeGauge = rand()%MaxTimeGauge;	//ランダムでいいの？
	Status[WAIT] = true;
	Mode = PREPARE;
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

bool CActor::SetSystemImg(CBImgBank* _bImgBank){
	
	SetBarImg(_bImgBank, HP_BAR);
	SetBarImg(_bImgBank, TIME_BAR1);
	SetBarImg(_bImgBank, TIME_BAR2);
	SetBarImg(_bImgBank, TIME_DEFFENCE);
	SetBarImg(_bImgBank, TIME_WAIT);
	SetBarImg(_bImgBank, TIME_PRAY);
	SetBarImg(_bImgBank, TIME_TRICK);

	SetExtraImg(_bImgBank);

	return true;
}
void CActor::SetBarImg(CBImgBank* _bImgBank, std::string _key) {
	BarImg[_key] = _bImgBank->GetImg(_key.c_str());
}

/*
void CActor::AddTrick(trick_tag const* _trick){
	TrickList.push_back(_trick);
}
*/

bool CActor::Main(){

	if (Alive){
		if (TimeGaugeForward()){
			Mode = ACTION;
			return true;
		}
	}		
/*		switch(Mode){
		case PLAN:
		case PREPARE:
			if (TimeGaugeForward()){
				Mode = ACTION;
				return true;
			}
			break;

		case ACTION:
			if (TimeGaugeForward()){
				Mode = PLAN;
				return true;
			}
			break;
	
		default:
			ErrorDx("Error->CActor::Main->unexpected Mode:%d", __FILE__, __LINE__, Mode);
		}
	}*/

	return false;
}

bool CActor::Do(){		//行動待機リスト上位のものから行動していく

	if (Alive){
		bool forReturn = false;
		switch (Mode){
		case BEFORE_PLAN:
			Mode = PLAN;
		case PLAN:
			forReturn = Plan();
			break;
		case ACTION:
			if (forReturn = Action()) {
				Mode = BEFORE_PLAN;
				forReturn = false;
			}
			break;
		default:
			ErrorDx("Error->CActor::Do->unexpected Mode:%d", __FILE__, __LINE__, Mode);
			return true;
		}

		//if (forReturn) TimeGaugeForward(); //同時行動時に行動が済んだキャラからゲージが0に戻る　//同時行動はまず起こらないため削除
		return forReturn;

	}else{
		return true;
	}

}

bool CActor::GetStatus(int _key){
	if (Status.find(_key) != Status.end()) {
		return Status[_key];
	} else {
		return false;
		//ERRORDX("NotFound Key:%s", _key);
	}
}


int CActor::Damaged(CActor* _attacker, trick_tag const* _trick){
	if (_trick==NULL) {
		ErrorDx("Error->CActor::Damage->_trick==NULL", __FILE__, __LINE__);
		return 0;
	}
	
	double damage  = _trick->Power + _attacker->GetAtk() - Def;	//$ダメージ計算式は要検討
	
	damage = CalcDamage(damage, _attacker, _trick);

	Hp = between(0, MaxHp, Hp-(int)damage);
	//死亡判定はCheckBarMoveではなくここですべきか？

	return (int)damage;
}

bool CActor::CheckBarMove(){	//Hpバーの移動終了を確認
	
	if (Alive && Hp==0) Alive = false;

	if (OldHp!=Hp) { //Hpバー減少中
		return false;
	}else{			//Hpバー減少終了
		if (!Alive && VisibleStatus == VISIBLE) {
			char tmp[256];
			sprintf_s(tmp, "%sは倒れた！", GetName().c_str());
			LogWindow->Add(tmp);
			VisibleStatus = CHANGING;
		}

		return (VisibleStatus != CHANGING);		//描画状況が安定してからtrueをかえす
	}
}

void CActor::ChangeValue(int _kind, int _powerPercent){
	char tmp[256];
	
	switch(_kind){
	case sideEffect_tag::ATK:
		Atk += (int)((double)Atk*_powerPercent/100);

		if (_powerPercent>0){
			sprintf_s(tmp, "  %sの攻撃力が上がった！", GetName().c_str());
			LogWindow->Add(tmp);
		}else if(_powerPercent<0){
			sprintf_s(tmp, "  %sの攻撃力が下がった！", GetName().c_str());
			LogWindow->Add(tmp);
		}
		break;

	case sideEffect_tag::DEF:
		Def += (int)((double)Def*_powerPercent/100);

		if (_powerPercent>0){
			sprintf_s(tmp, "  %sの防御が上がった！", GetName().c_str());
			LogWindow->Add(tmp);
		}else if(_powerPercent<0){
			sprintf_s(tmp, "  %sの防御力が下がった！", GetName().c_str());
			LogWindow->Add(tmp);
		}
		break;

	default:
		ERRORDX("StatusKind doesn't match any status. :%s", _kind);
		return;
	}
}


void CActor::Draw_Sub(int _dx, int _dy){
	CVector barTop(Rect.Center().x, Rect.Center().y + (IsPlayer()? 45: 80));
	CVector barSize;

	//HpBar
		barSize = GetGraphSize(BarImg[HP_BAR]);
			  DrawBox((int)(-1+barTop.x-barSize.x/2+_dx), (int)(-1+barTop.y+_dy), (int)(1+barTop.x+barSize.x/2+_dx), (int)(1+barTop.y+barSize.y+_dy), BLUE, true);
		DrawRectGraph((int)(barTop.x-barSize.x/2+_dx)   , (int)(barTop.y+_dy), 0, 0, (int)(barSize.x*OldHp/MaxHp), (int)barSize.y, BarImg[HP_BAR], false, false);

	//TimeBar
		const CVector TIME_BAR_SIZE(50, 10);
		barTop.x -= barSize.x/2;		//HpBarに左端はそろえる
		barTop.y += barSize.y + 5;

		int img;
		if (Mode == PLAN || GetStatus(WAIT)) {
			img = BarImg[TIME_WAIT];
		} else if (GetStatus(DEFFENCE)) {
			img = BarImg[TIME_DEFFENCE];
		} else if (GetStatus(PRAY)) {
			img = BarImg[TIME_PRAY];
		} else {
			img = BarImg[TIME_TRICK];
		}

		if (Mode==PREPARE && !GetStatus(DEFFENCE)) SetDrawBright(150,150,150);		
	  	DrawBox((int)(-1+barTop.x+_dx), (int)(-1+barTop.y+_dy),(int)(1+barTop.x+TIME_BAR_SIZE.x*MaxTimeGauge/100.0+_dx), (int)(1+barTop.y+TIME_BAR_SIZE.y+_dy), BLUE, true);
		DrawRectGraph((int)(barTop.x+_dx), (int)(barTop.y+_dy), 0, 0, (int)(TIME_BAR_SIZE.x*(MaxTimeGauge-TimeGauge)/100.0), (int)TIME_BAR_SIZE.y, img, false, false);
	
	//OldHpとHpのギャップを埋める
		if (OldHp>Hp) OldHp--;
		else if (OldHp<Hp) OldHp++;

	DrawCenterString((int)(Rect.Center().x), (int)Rect.Center().y, (Mode==PLAN?"PLAN":(Mode==PREPARE?"PREPARE":"ACTION")) , BLACK, true);  

	SetDrawBright(255,255,255);
	SetDrawBlendMode( DX_BLENDMODE_NOBLEND , 0 ) ;
}



bool CActor::TimeGaugeForward(){
	if (TimeGauge==0) {
		TimeGauge = MaxTimeGauge;
		Mode = (mode_tag)((Mode+1) % MODE_NUM);
	}
	TimeGauge-=SpdPer;
	if (TimeGauge<=0){
		TimeGauge = 0;
		return true;
	}
	return false;
}