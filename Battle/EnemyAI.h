////多重インクルード防止（インクルードガード）//
#ifndef ENEMYAI_H						////
#define ENEMYAI_H						////
////////////////////////////////////////////////

class CActor;
class CEnemy;

#include "EnemyPlanner.h"

class CEnemyAI{		//EnemySpeciesが持つ、plannerやTargeterを束ねる
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
		//行動選択肢とその発動比を並べたリスト。
		//行動計算の為にAI.plannerとAI.Targetにポインタを渡しておく

	//全アクターへのアクセスを持たせておく（戦闘開始ごとに更新）（EnemyAIは橋渡しなので実際に持つ必要はない）
		const CActor* const* Actor;
		int PLAYER_NUM;
		int ENEMY_NUM;

};

////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////