////多重インクルード防止（インクルードガード）//
#ifndef ENEMYAI_H						////
#define ENEMYAI_H						////
////////////////////////////////////////////////

class CActor;
class CEnemy;

#include "EnemyPlanner.h"

class CEnemyAI{		//EnemySpeciesが持つ、PlannerやTargeterを束ねる
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
		Planner->BattleReady(_actorList, _playerNum, _enemyNum);
		Actor = _actorList;
		PLAYER_NUM = _playerNum;
		ENEMY_NUM = _enemyNum;
	}

	CEnemyPlanner* SetPlanner(CEnemyPlanner* _planner){
		Planner = _planner;
		return _planner;
	}

	int GetPlan(const CEnemy* _enemy) { return Planner->GetPlan(_enemy); }
	int GetTarget(const CEnemy* _enemy);

private:
	CEnemyPlanner* Planner;

	//全アクターへのアクセスを持たせておく（戦闘開始ごとに更新）（EnemyAIは橋渡しなので実際に持つ必要はない）
		const CActor* const* Actor;
		int PLAYER_NUM;
		int ENEMY_NUM;

};

////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////