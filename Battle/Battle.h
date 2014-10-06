////多重インクルード防止（インクルードガード）//
#ifndef BATTLE_H							////
#define BATTLE_H							////
////////////////////////////////////////////////

#include "BImgBank.h"
#include "TrickManager.h"
#include "Player.h"
#include "Enemy.h"
#include "SpeciesManager.h"

#include "../Main/TextBox.h"
#include "../Main/TextWrap.h"
#include "../Main/CmdList.h"
#include "../Main/CmdManager.h"
#include "../Field/EveManager.h"
#include "../Field/Map.h"

#include "../Main/WorldManager.h"

#include <vector>
#include <string>
#include <queue>
#include <map>


class CBattle : public CWorldManager{
public:

	CBattle();

	bool Init();
	void Term();
	void BattleStart(int* _result, CFlagSet* _flagset_p, CField* _field_p, CMap* _map_p, CEveManager* _evemanager_p);
	void Draw(bool _screenflip=false, bool _textshowingstop=false, int dx=0, int dy=0, bool _playeralsoshake=false);

	void ChangeTextMode(bool _box, const char* _eventtext = NULL);

	int GetActorNum(){return ACTOR_NUM;}

	void SetBackGround(const char* _pickey);
	void SetPlayer();
	void SetPlayer(const int _playerNum, ...);
	void SetEnemy(const int _enemyNum, ...);
	void SetEnemy(std::vector<std::string> _enemyList);
	

	//戦闘用
		void Damage(int _attacker_actorindex, int _target_actorindex, trick_tag const* _trick);

	//グラフィック用 for ImgBank
		enum{	//システム画像	
			MENU_CURSOR,
			HP_BAR,
			TIME_BAR1,
			TIME_BAR2,
		};
		enum{	//キャラクタ画像
			//FACE1,
			//FACE2,
			CHARA_IMG_NUM
		};

	//ターゲット選択マーカー
		class CTargetMarker{
		public:
			void Init(int _actornum, int _img){
				Img = _img;
				Visible = false;
				EnemySide = true;
				Index = 0;
			}
			void SetVisible(bool _visible){Visible = _visible;}
			void SetSide(bool _enemy){EnemySide = _enemy;}
			bool GetSide(){return EnemySide;}
			void SetIndex(int _index){Index = between(0, (EnemySide? MAX_ENEMY_NUM: MAX_PLAYER_NUM), _index);}
			void Move(int _dir); 
			void Decide(CBattle* _battle, int _actorindex, bool _deadok);
			void Draw(int dx = 0, int dy = 0);

		private:
			bool Visible;
			int Img;
			bool EnemySide;	//敵側か味方側か
			int Index;		//敵ごと味方ごとの通し番号
			int ActorNum;
		}TargetMarker;

private:
	//メンバ関数
		int MainLoop();
		void BattleFinish();
	
	//メンバ変数
		//所持インスタンスとポインタ
			CCmdList B_CmdList;				
			CBattleCmdManager B_CmdManager;	
		
			CTextBox* TextBox;
			CTextBox TextBox1;
			CTextWrap TextWrap1;
		
			CEveManager* EveManager_p;	//ラップモード時にオリジナルイベント読み込み用
			CFlagSet* FlagSet_p;

		
		//Actors
			//CPlayer Player[MAX_PLAYER_NUM];	//消す
			//CEnemy Enemy[MAX_ENEMY_NUM];///消す

			CPlayer* Player;
			CEnemy* Enemy;
			CActor** Actor;
			int ACTOR_NUM;
			int PLAYER_NUM;
			int ENEMY_NUM;
			//vector<CEnemy>


		//グラフィック
			int Img_BattleBackGround;	//戦闘背景

		//重要な所持インスタンス
			CBImgBank BImgBank;
			CPlayerSpeciesManager PlayerSpeciesManager;
			CEnemySpeciesManager EnemySpeciesManager;
			CTrickManager TrickManager;
			std::queue <CActor*> ActionQueue;	//行動待機リスト


};


////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////