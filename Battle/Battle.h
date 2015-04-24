////多重インクルード防止（インクルードガード）//
#ifndef BATTLE_H							////
#define BATTLE_H							////
////////////////////////////////////////////////

#include "BImgBank.h"
#include "TrickManager.h"
#include "Player.h"
#include "Enemy.h"
#include "LogWindow.h"

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

class PlayerSpeciesManager;
class EnemySpeciesManager;


class CBattle : public CWorldManager{
public:
	static CBattle* GetInstance(){
		static CBattle instance;
		return &instance;
	}

	~CBattle(){
		DESTRUCTED;	
	}

	bool Init();
	void BattleReady(CFlagSet* _flagset_p, CMap* _map_p, CEveManager* _evemanager_p);
	void BattleStart(int* _result, CCmdList* _fieldcmdlist_p);
	void BattleSetting(const char* _wincommand, const char* _losecommand);
	bool CheckEncount(int _mapnum, int _chipnum);
	
	void Draw(bool _screenflip=false, bool _textshowingstop=false, int dx=0, int dy=0, bool _playeralsoshake=false);

	void ChangeTextMode(bool _box, const char* _eventtext = NULL);

	int GetActorNum()const{ return ACTOR_NUM; }

	void SetBackGround(const char* _pickey);
	void SetBackGround(int _mapnum, int _chipnum=-1);

	void SetPlayer();
	void SetPlayer(const int _playerNum, ...);
	void SetEnemy(const int _enemyNum, ...);
	void SetEnemy(std::vector<std::string> _enemyList);
	void SetEnemy(std::vector<CEnemySpecies*> _enemyParty);
	

	//戦闘用
		void ManageAttack(int _attackerActorIndex, int _targetActorIndex, trick_tag const* _trick);

		//アテンション関係
			void AddAttention(int _enemyIndex, int _playerIndex, int _value){
				NUM_MAX_CHECK(_enemyIndex, ENEMY_NUM,);
				NUM_MAX_CHECK(_playerIndex, PLAYER_NUM,);
				Enemy[_enemyIndex].AddAttention(_playerIndex, _value);
			}
			void SetAttention(int _enemyIndex, int _playerIndex, int _value){
				NUM_MAX_CHECK(_enemyIndex, ENEMY_NUM,);
				NUM_MAX_CHECK(_playerIndex, PLAYER_NUM,);
				Enemy[_enemyIndex].SetAttention(_playerIndex, _value);
			}

	
	//ターゲット選択マーカー
		class CTargetMarker{
		public:
			CTargetMarker(){
				CONSTRUCTED;	
			}
			~CTargetMarker(){
				DESTRUCTED;	
			}

			void SetImage(int _img);
			void Init(int _actornum, int _playernum, int _enemynum);

			void SetVisible(bool _visible){	Visible = _visible;}
			void SetSide(bool _enemy){		EnemySide = _enemy;}
			bool GetSide(){	return EnemySide;}
			void SetIndex(int _index){		Index = between(0, (EnemySide? ENEMY_NUM: PLAYER_NUM), _index);}
			void SetDeadOk(bool _deadok){	DeadOk = _deadok;}

			void CheckNowIndex(CBattle* _battle);
			void Move(int _dir, CBattle* _battle, int _count=0); 
			void Decide(CBattle* _battle, int _actorIndex);
			void Draw(int dx = 0, int dy = 0);

		private:
			bool Visible;
			int Img;
			int ImgSizeX, ImgSizeY;
			bool EnemySide;	//敵側か味方側か
			int Index;		//敵ごと味方ごとの通し番号
			bool DeadOk; //死人を選択対象とするか

			int ACTOR_NUM;
			int PLAYER_NUM;
			int ENEMY_NUM;
			int Status; //0...ふよふよ　1...確定つなぎ　2...確定演出中
		}TargetMarker;

private:
	//メンバ関数
		void StartEffect();
		int MainLoop();
		void BattleFinish(int &_result, CCmdList* _fieldcmdlist);
		int ResultCheck();

	
	//メンバ変数
		bool Ready;

		//所持インスタンスとポインタ
			CCmdList B_CmdList;				
			CBattleCmdManager B_CmdManager;	
		
			CTextBox* TextBox;
			CTextBox TextBox1;
			CTextWrap TextWrap1;
			CLogWindow LogWindow;
		
			CEveManager* EveManager_p;	//ラップモード時にオリジナルイベント読み込み用
			CFlagSet* FlagSet_p;

		//Actors
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
			CPlayerSpeciesManager* PlayerSpeciesManager;
			CEnemySpeciesManager* EnemySpeciesManager;
			CTrickManager* TrickManager;	//singleton使う関係上ポインタで保持
			std::queue <CActor*> ActionQueue;	//行動待機リスト

		//戦闘設定
			char WinCommand[256];
			char LoseCommand[256];

	//単一性を保証(Singleton)//////////////////////////////////////////
		CBattle();
		CBattle(const CBattle& hoge);
		CBattle& operator=(const CBattle& hoge);
	//////////////////////////////////////////////////////////////////

};


////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////