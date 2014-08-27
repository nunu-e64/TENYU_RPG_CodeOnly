////多重インクルード防止（インクルードガード）//
#ifndef ENEMYPLANMANAGER_H					////
#define ENEMYPLANMANAGER_H					////
////////////////////////////////////////////////

#include "Actor.h"
#include <map>

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

	//void MakePlan();
	void Init();
	void MakePlan(int _enemyIndex, int _borderHp, int _choice1, int _choice2);

	int Plan(CEnemy* _enemy);


	class CEnemyPlanner{
	public:
		int PlanType;
		std::map <char256, int> Value;
			//mapでは存在しないキーでアクセスしようとした場合、値をデフォルトコンストラクタで初期化し登録してくれやがる
	};
	

private:
	std::vector <CEnemyPlanner> EnemyPlanner;
	CActor** Actor;
	int ACTOR_NUM;

	int Calc_HpBorder(float _hpBorder, int choice1, int choice2);
	int Calc_PlayerNum(int choice_1player, int choice_2player, int choice_3player);


	//単一性を保証(Singleton)
		CEnemyPlanManager(){}
		CEnemyPlanManager(const CEnemyPlanManager& hoge);
		CEnemyPlanManager& operator=(const CEnemyPlanManager& hoge);
};



////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////
