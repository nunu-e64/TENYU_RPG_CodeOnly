////多重インクルード防止（インクルードガード）//
#ifndef ACTOR_H							////
#define ACTOR_H							////
////////////////////////////////////////////////

#include <vector>
#include <map>

class CTextBox;		//前方宣言
class CCmdList;

class CActor{
public:
	CActor(){}	//Actor = new CActor* [MAX_PLAYER+MAX_ENEMY];}
	~CActor(){}	//delete [] Actor;}

	//初期設定関連
		void FirstSet(int _index, CTextBox** _textbox, CCmdList* _cmdlist, std::map <int,int> *_imgbank);
		void SetValue(int _atk, int _def, double _spd, int _maxhp);
		void SetRect(int _cx, int _cy);
		void SetImg(int _img);
		void AddTrick(trick_tag const* _trick);
		void ClearTrick(){TrickList.clear();}


	bool Main();
	bool Do();
	virtual void Draw(int dx=0, int dy=0)=0;	//純粋仮想関数…親クラス側に定義を書く必要が無くなる

	//基礎情報
		int GetIndex(){return Index;}
		
	//戦闘関連
		bool GetAlive(){return Alive;}
		//int GetDef(){return Def;}
		int GetAtk(){return Atk;}
		int GetHp(){return Hp;}
		int GetMaxHp(){return MaxHp;}
		trick_tag const* GetNowTrick(){return NowTrick;}

		int Damage(CActor* _attacker, trick_tag const* _trick);
		bool DeadCheck();	//死亡確認が済んだらTrue
		void SetTarget(int _target){Target=_target;}

	//ほか
		CRect GetRect(){return Rect;}

protected:
	//メンバ関数
		virtual void Init(){}	//Playerのみ必要なので。Enemyの時にはこっちを通る

		virtual bool Plan()=0;
		virtual bool Action()=0;

		bool TimeGaugeForward();

		//void DamageStep(int _targetnum, ...);
		//void DamageStep(CActor* _target);


	//基礎情報
		int ActorIndex;	//Actor全体の通し番号
		int Index;	//Player,Enemyごとの通し番号

	//戦闘関連
		bool Alive;
		bool Visible;
		int Atk;
		int Def;
		double Spd;		//%
		int Hp;		int OldHp;	//描画用
		int MaxHp;
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
		std::vector <trick_tag const*> TrickList;		//技リスト
		trick_tag const* NowTrick;		//「const が * よりも前方にあれば、指し示す先の値が書き換えられない。後方にあれば、ポインタ変数自体が書き換えられない」
		int Target;

	//包含クラス
		CTextBox** B_TextBox_pp;
		CCmdList* CmdList;

	//グラフィック
		CRect Rect;
		int Img_hpbar;
		int Img_timebar[2];
		int Img;
		//...
};


////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////