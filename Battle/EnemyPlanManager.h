////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#ifndef ENEMYPLANMANAGER_H					////
#define ENEMYPLANMANAGER_H					////
////////////////////////////////////////////////

#include "Actor.h"
#include <map>
#include <string>

class CEnemy;

class CEnemyPlanManager{
public:
	static CEnemyPlanManager* GetInstance(){
		static CEnemyPlanManager EnemyManager;
		return &EnemyManager;
	}
	void SetActor_p(CActor** _actorList){
		Actor = _actorList;
	}

	void Init(int _playernum, int _enemynum);
	void MakePlan(int _enemyIndex, int _borderHp, int _choice1, int _choice2);
	int Plan(CEnemy* _enemy);

private:
	class CEnemyPlanner{
	public:
		int PlanType;
		std::map <std::string, int> Value;
			//map�ł͑��݂��Ȃ��L�[�ŃA�N�Z�X���悤�Ƃ����ꍇ�A�l���f�t�H���g�R���X�g���N�^�ŏ��������o�^���Ă���₪��
	};
	std::vector <CEnemyPlanner> EnemyPlanner;	//map�ł���

	CEnemy* NowEnemy;

	//�S�A�N�^�[�ւ̃A�N�Z�X���������Ă���
		CActor** Actor;
		int PLAYER_NUM;
		int ENEMY_NUM;

	//��̓I��AI�\�z�p�֐�
		int Calc_HpBorder(float _hpBorder, int choice1, int choice2);
		int Calc_PlayerNum(int choice_1player, int choice_2player, int choice_3player);


	//�P�ꐫ��ۏ�(Singleton)
		CEnemyPlanManager(){}
		CEnemyPlanManager(const CEnemyPlanManager& hoge);
		CEnemyPlanManager& operator=(const CEnemyPlanManager& hoge);
};



////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										////
////////////////////////////////////////////////
