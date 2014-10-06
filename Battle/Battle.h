////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
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
	void BattleStart(int* _result, CFlagSet* _flagset_p, CCmdList* _fieldcmdlist_p, CMap* _map_p, CEveManager* _evemanager_p);
	void BattleSetting(const char* _winmessage, const char* _losemessage);
	
	void Draw(bool _screenflip=false, bool _textshowingstop=false, int dx=0, int dy=0, bool _playeralsoshake=false);

	void ChangeTextMode(bool _box, const char* _eventtext = NULL);

	int GetActorNum(){return ACTOR_NUM;}

	void SetBackGround(const char* _pickey);
	void SetPlayer();
	void SetPlayer(const int _playerNum, ...);
	void SetEnemy(const int _enemyNum, ...);
	void SetEnemy(std::vector<std::string> _enemyList);
	

	//�퓬�p
		void Damage(int _attacker_actorindex, int _target_actorindex, trick_tag const* _trick);

	//�^�[�Q�b�g�I���}�[�J�[
		class CTargetMarker{
		public:
			void Init(int _actornum, int _playernum, int _enemynum, int _img);

			void SetVisible(bool _visible){Visible = _visible;}
			void SetSide(bool _enemy){EnemySide = _enemy;}
			bool GetSide(){return EnemySide;}
			void SetIndex(int _index){Index = between(0, (EnemySide? ENEMY_NUM: PLAYER_NUM), _index);}
			void Move(int _dir); 
			void Decide(CBattle* _battle, int _actorindex, bool _deadok);
			void Draw(int dx = 0, int dy = 0);

		private:
			bool Visible;
			int Img;
			bool EnemySide;	//�G������������
			int Index;		//�G���Ɩ������Ƃ̒ʂ��ԍ�
			int ACTOR_NUM;
			int PLAYER_NUM;
			int ENEMY_NUM;
		}TargetMarker;

private:
	//�����o�֐�
		int MainLoop();
		void BattleFinish(int _result, CCmdList* _fieldcmdlist);
		int ResultCheck();

	
	//�����o�ϐ�
		//�����C���X�^���X�ƃ|�C���^
			CCmdList B_CmdList;				
			CBattleCmdManager B_CmdManager;	
		
			CTextBox* TextBox;
			CTextBox TextBox1;
			CTextWrap TextWrap1;
		
			CEveManager* EveManager_p;	//���b�v���[�h���ɃI���W�i���C�x���g�ǂݍ��ݗp
			CFlagSet* FlagSet_p;

		//Actors
			CPlayer* Player;
			CEnemy* Enemy;
			CActor** Actor;
			int ACTOR_NUM;
			int PLAYER_NUM;
			int ENEMY_NUM;
			//vector<CEnemy>


		//�O���t�B�b�N
			int Img_BattleBackGround;	//�퓬�w�i

		//�d�v�ȏ����C���X�^���X
			CBImgBank BImgBank;
			CPlayerSpeciesManager PlayerSpeciesManager;
			CEnemySpeciesManager EnemySpeciesManager;
			CTrickManager TrickManager;
			std::queue <CActor*> ActionQueue;	//�s���ҋ@���X�g

		//�퓬�ݒ�
			char WinMessage[256];
			char LoseMessage[256];

};


////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										////
////////////////////////////////////////////////