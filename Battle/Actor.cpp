#include "../Define.h"
#include "Actor.h"
#include "BImgBank.h"
#include "../Main/TextBox.h"
#include "LogWindow.h"

int CActor::HpFontHandle = -1;

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
	if (HpFontHandle==-1) HpFontHandle = CreateFontToHandle(NULL , 10, -1) ;	//HpBarに表示するHP用のフォント作成

	MaxTimeGauge = 100;	//%
	TimeGauge = rand()%MaxTimeGauge;	//ランダムでいいの？

	Status[WAIT] = true;
	Mode = PREPARE;

	Target = -1;
	NowTrick = NULL;

	Dx=0; Dy=0;

	FirstSet2();
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

		//if (forReturn) TimeGaugeForward(); //同時行動時に行動が済んだキャラからゲージが0に戻る	//同時行動はまず起こらないため削除
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
	
	double damage = _trick->Power * _attacker->GetAtk() / Def;	//$ダメージ計算式は要検討

	for (unsigned int i = 0; i < _attacker->StatusChangerList.size(); i++) {
		switch (_attacker->StatusChangerList[i].StatusKind) {
		case sideEffect_tag::ATK:
			damage *= (100 + _attacker->StatusChangerList[i].Power) / 100.0;
			break;
		}
	}

	for (unsigned int i = 0; i < StatusChangerList.size(); i++) {
		switch (StatusChangerList[i].StatusKind) {
		case sideEffect_tag::DEF:
			damage /= (100+StatusChangerList[i].Power)/100.0;
			break;
		}
	}

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
			LogWindow->Add("%sは倒れた！", GetName().c_str());
			VisibleStatus = CHANGING;
		}

		return (VisibleStatus != CHANGING);		//描画状況が安定してからtrueをかえす
	}
}

void CActor::AddStatusChanger(int _kind, int _powerPercent, int _time) {
	
	statusChanger_tag tmp;
	tmp.StatusKind = _kind;
	tmp.Power = _powerPercent;
	tmp.Time = _time;
	tmp.Img = 0;  //@TODO:ImgBankから画像入手

	StatusChangerList.push_back(tmp);


	//ログ出力
		char chtmp[256];
		switch (_kind) {
		case sideEffect_tag::ATK:
			if (_powerPercent>0) {
				mystrcpy(chtmp, "  %sの攻撃力が%d％上がった！");
			} else if (_powerPercent<0) {
				mystrcpy(chtmp, "  %sの攻撃力が%d％下がった！");
			}
			break;

		case sideEffect_tag::DEF:
			if (_powerPercent>0) {
				mystrcpy(chtmp, "  %sの防御力が%d％上がった！");
			} else if (_powerPercent<0) {
				mystrcpy(chtmp, "  %sの防御力が%d％下がった！");
			}
			break;

		default:
			ERRORDX("StatusKind doesn't match any status. :%s:%d", Name.c_str(), _kind);
			return;
		}

		LogWindow->Add(chtmp, Name.c_str(), abs(_powerPercent));

}

void CActor::ChangeValue(int _kind, int _powerPercent){
	
	switch(_kind){
	case sideEffect_tag::ATK:
		Atk += (int)((double)Atk*_powerPercent/100);

		if (_powerPercent>0){
			LogWindow->Add("  %sの攻撃力が上がった！", GetName().c_str());
		}else if(_powerPercent<0){
			LogWindow->Add("  %sの攻撃力が下がった！", GetName().c_str());
		}
		break;

	case sideEffect_tag::DEF:
		Def += (int)((double)Def*_powerPercent/100);

		if (_powerPercent>0){
			LogWindow->Add("  %sの防御が上がった！", GetName().c_str());
		}else if(_powerPercent<0){
			LogWindow->Add("  %sの防御力が下がった！", GetName().c_str());
		}
		break;

	default:
		ERRORDX("StatusKind doesn't match any status. :%s", _kind);
		return;
	}
}


void CActor::Draw_Sub(int _dx, int _dy){
	CVector barPos;
	CVector barSize;

	//HpBar
		barSize = GetGraphSize(BarImg[HP_BAR]);
		barPos = CVector(Rect.Center().x-barSize.x/2, Rect.Center().y + (IsPlayer()? 45: 80));
			  DrawBox((int)(-1+barPos.x+_dx), (int)(-1+barPos.y+_dy), (int)(1+barPos.x+barSize.x+_dx), (int)(1+barPos.y+barSize.y+_dy), BLUE, true);
		DrawRectGraph((int)(barPos.x+_dx)   , (int)(barPos.y+_dy), 0, 0, (int)(barSize.x*OldHp/MaxHp), (int)barSize.y, BarImg[HP_BAR], false, false);

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
		DrawFormatStringToHandle((int)(2+barPos.x+_dx), (int)(barPos.y+_dy), GetColor(250, 200, 50), HpFontHandle,"%d", Hp);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, -1);

	//TimeBar
		const CVector TIME_BAR_SIZE(50, 10);
		barPos.y += barSize.y + 5;

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

		if (Mode==PREPARE && !GetStatus(DEFFENCE)) SetDrawBright(180,180,180);		
	  	DrawBox((int)(-1+barPos.x+_dx), (int)(-1+barPos.y+_dy),(int)(1+barPos.x+TIME_BAR_SIZE.x*MaxTimeGauge/100.0+_dx), (int)(1+barPos.y+TIME_BAR_SIZE.y+_dy), BLUE, true);
		DrawRectGraph((int)(barPos.x+_dx), (int)(barPos.y+_dy), 0, 0, (int)(TIME_BAR_SIZE.x*(MaxTimeGauge-TimeGauge)/100.0), (int)TIME_BAR_SIZE.y, img, false, false);
	
	//StatusChangerIcon
		barPos.y += TIME_BAR_SIZE.y + 5;
		for (unsigned int i = 0; i < StatusChangerList.size(); i++) {
			DrawGraph(barPos.x +i*GetGraphSize(StatusChangerList[i].Img).x, barPos.y, StatusChangerList[i].Img, true);
		}

	//OldHpとHpのギャップを埋める
		if (OldHp>Hp) OldHp--;
		else if (OldHp<Hp) OldHp++;

	DrawCenterString((int)(Rect.Center().x), (int)Rect.Center().y, (Mode==PLAN?"PLAN":(Mode==PREPARE?"PREPARE":"ACTION")) , BLACK, true);  //@TEST//

	SetDrawBright(255,255,255);
	SetDrawBlendMode( DX_BLENDMODE_NOBLEND , 0 ) ;
}



bool CActor::TimeGaugeForward(){
	if (TimeGauge==0) {
		TimeGauge = MaxTimeGauge;
		Mode = (mode_tag)((Mode+1) % MODE_NUM);
	}
	
	TimeGauge-=SpdPer;

	for (std::vector<statusChanger_tag>::iterator it = StatusChangerList.begin(); it != StatusChangerList.end();) {
		if (--(*it).Time <= 0) { 
			it = StatusChangerList.erase(it); // erase()の戻り値をitで受ける！
		} else {
			++it;
		}
	}

	if (TimeGauge<=0){
		TimeGauge = 0;
		return true;
	}
	return false;
}