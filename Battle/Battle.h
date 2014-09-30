////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#ifndef BATTLE_H							////
#define BATTLE_H							////
////////////////////////////////////////////////

#include "TrickManager.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemySpeciesManager.h"

#include "../Main/TextBox.h"
#include "../Main/TextWrap.h"
#include "../Main/CmdList.h"
#include "../Main/CmdManager.h"
#include "../Field/EveManager.h"
#include "../Field/Map.h"

#include "../Main/WorldManager.h"

#include <queue>
#include <map>


class CBattle : public CWorldManager{
public:

	CBattle();
	void Init();
	void SetEnemy(const int _enemyNum, ...);

	void Battle(int* _result, CFlagSet* _flagset_p, CField* _field_p, CMap* _map_p, CEveManager* _evemanager_p);

	enum{ACTOR_NUM = MAX_PLAYER + MAX_ENEMY};

	void Draw(bool _screenflip=false, bool _textshowingstop=false, int dx=0, int dy=0, bool _playeralsoshake=false);
	void ChangeTextMode(bool _box, const char* _eventtext = NULL);

	//�퓬�p
		void Damage(int _attacker_actorindex, int _target_actorindex, trick_tag const* _trick);

	//�O���t�B�b�N�p for ImgBank
		enum{	//�V�X�e���摜	
			MENU_CURSOR,
			HP_BAR,
			TIME_BAR1,
			TIME_BAR2,
		};
		enum{	//�L�����N�^�摜
			//FACE1,
			//FACE2,
			CHARA_IMG_NUM
		};

	//�^�[�Q�b�g�I���}�[�J�[
		class CTargetMarker{
		public:
			void Init(int _img){
				Img = _img;
				Visible = false;
				EnemySide = true;
				Index = 0;
			}
			void SetVisible(bool _visible){Visible = _visible;}
			void SetSide(bool _enemy){EnemySide = _enemy;}
			bool GetSide(){return EnemySide;}
			void SetIndex(int _index){Index = between(0, (EnemySide? MAX_ENEMY: MAX_PLAYER), _index);}
			void Move(int _dir); 
			void Decide(CBattle* _battle, int _actorindex, bool _deadok);
			void Draw(int dx = 0, int dy = 0);

		private:
			bool Visible;
			int Img;
			bool EnemySide;	//�G������������
			int Index;		//�G���Ɩ������Ƃ̒ʂ��ԍ�
		}TargetMarker;

private:
	//�����o�֐�
		int MainLoop();
	
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
			CPlayer Player[MAX_PLAYER];
			CEnemy Enemy[MAX_ENEMY];
			CActor* Actor[ACTOR_NUM];
			int ENEMY_NUM;
			//vector<CEnemy>


		//�O���t�B�b�N
			int Img_BattleBackGround;	//�퓬�w�i
			std::map <int, int> ImgBank;
			std::vector <int> PlayerImgBank;
			std::vector <int> EnemyImgBank;

		//�d�v�ȏ����C���X�^���X
			CEnemySpeciesManager EnemySpeciesManager;
			CTrickManager TrickManager;
			std::queue <CActor*> ActionQueue;	//�s���ҋ@���X�g


};


////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										////
////////////////////////////////////////////////