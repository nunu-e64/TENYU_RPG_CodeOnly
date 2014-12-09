////多重インクルード防止（インクルードガード）//
#ifndef ACTOR_H								////
#define ACTOR_H								////
////////////////////////////////////////////////

#include <vector>
#include <map>
#include <string>

#include "Species.h"

class CTextBox;		//前方宣言
class CCmdList;
class CBImgBank;

class CActor : public virtual CSpecies{
public:
	CActor(const CSpecies &obj) : CSpecies(obj){
		CONSTRUCTED
	}

	CActor(){
		CONSTRUCTED
	}
	~CActor(){
		DESTRUCTED
	}

	//初期設定関連
		void FirstSet(int _playernum, int _enemynum, int _index, CTextBox** _textbox, CCmdList* _cmdlist);
		void SetRect(int _cx, int _cy);
		//void SetImg(int _img); //使ってない
		bool SetSystemImg(CBImgBank* _bimgbank);
		void AddTrick(trick_tag const* _trick);
		//void ClearTrick(){TrickList.clear();} //使ってない

	bool Main();
	bool Do();
	virtual void Draw(int _dx=0, int _dy=0)=0;	//純粋仮想関数…親クラス側に定義を書く必要が無くなる

	//基礎情報
		int GetIndex()const{return Index;}
		
	//戦闘関連
		void SetVisible(bool _visible){Visible = _visible;}
		bool GetVisible()const{return Visible;}
		bool GetAlive()const{return Alive;}
		//int GetDef(){return Def;}
		int GetAtk()const{return Atk;}
		int GetHp()const{return Hp;}
		int GetMaxHp()const{return MaxHp;}
		trick_tag const* GetNowTrick()const{return NowTrick;}

		int Damage(CActor* _attacker, trick_tag const* _trick);
		bool DeadCheck();	//死亡確認が済んだらTrue
		void SetTarget(int _target){Target=_target;}

	//その他
		CRect GetRect()const{return Rect;}

protected:
	//メンバ関数
		virtual void Init(){}	//Playerのみ必要なので。Enemyの時にはこっちを通る
		void Draw_Sub(int _dx=0, int _dy=0);

		virtual bool Plan()=0;
		virtual bool Action()=0;

		bool TimeGaugeForward();

		//void DamageStep(int _targetnum, ...);
		//void DamageStep(CActor* _target);


	//基礎情報
		int PLAYER_NUM;
		int ENEMY_NUM;
		int ActorIndex;	//Actor全体の通し番号
		int Index;	//Player,Enemyごとの通し番号

	//戦闘関連
		bool Alive;
		bool Visible;
		//int Hp;
		int OldHp;	//描画用
		double SpdPer;
		double TimeGauge;	//0~100%
		int Accident;	//状態異常やステータス変動

		enum mode_tag{
			STAY,
			PLAN,
			PREPARE,
			ACTION,
			MODE_NUM
		}Mode;		//待機や詠唱,行動選択など

	//技関連
		trick_tag const* NowTrick;		//「const が * よりも前方にあれば、指し示す先の値が書き換えられない。後方にあれば、ポインタ変数自体が書き換えられない」
		int Target;

	//インスタンスのポインタ保持用
		CTextBox** B_TextBox_pp;
		CCmdList* CmdList;

	//グラフィック
		CRect Rect;
		int Img_hpbar;
		int Img_timebar[2];
		int Dx; int Dy;
};


////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////