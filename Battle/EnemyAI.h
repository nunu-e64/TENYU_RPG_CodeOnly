////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#ifndef ENEMYAI_H						////
#define ENEMYAI_H						////
////////////////////////////////////////////////

class CActor;
class CEnemy;

#include "EnemyPlanner.h"

class CEnemyAI{		//EnemySpecies�����Aplanner��Targeter�𑩂˂�
public: 
	CEnemyAI(){
		CONSTRUCTED;
		Actor = NULL;
		PLAYER_NUM = 0;
		ENEMY_NUM = 0;
	}

	~CEnemyAI(){
		DESTRUCTED;	
	}

	void BattleReady(const CActor* const* _actorList, const int _playerNum, const int _enemyNum){
		planner->BattleReady(_actorList, _playerNum, _enemyNum);
		Actor = _actorList;
		PLAYER_NUM = _playerNum;
		ENEMY_NUM = _enemyNum;
	}
	bool AddRandomPlanSet(const unsigned int _index, std::vector<std::pair<int, int> > _plan_list);

	CEnemyPlanner* SetPlanner(CEnemyPlanner* _planner){
		planner = _planner;
		planner->SetRandomPlanSet(&RandomPlanSet);
		return _planner;
	}

	int GetPlan(const CEnemy* _enemy) { return planner->GetPlan(_enemy); }
	int GetTarget(const CEnemy* _enemy);

private:
	CEnemyPlanner* planner;

	std::map <int, std::vector<std::pair<int, int> > > RandomPlanSet;
		//RandomPlanSet[index] = (choice, probability)
		//�s���I�����Ƃ��̔��������ׂ����X�g�B
		//�s���v�Z�ׂ̈�AI.planner��AI.Target�Ƀ|�C���^��n���Ă���

	//�S�A�N�^�[�ւ̃A�N�Z�X���������Ă����i�퓬�J�n���ƂɍX�V�j�iEnemyAI�͋��n���Ȃ̂Ŏ��ۂɎ��K�v�͂Ȃ��j
		const CActor* const* Actor;
		int PLAYER_NUM;
		int ENEMY_NUM;

};

////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										////
////////////////////////////////////////////////