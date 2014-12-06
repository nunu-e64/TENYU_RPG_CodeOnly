////多重インクルード防止（インクルードガード）//
#ifndef ENEMYPLANMANAGER_H					////
#define ENEMYPLANMANAGER_H					////
////////////////////////////////////////////////

#include "EnemyPlanner.h"
#include "Actor.h"

class CEnemy;

class CEnemyPlanManager{
public:
	static CEnemyPlanManager* GetInstance(){
		static CEnemyPlanManager EnemyManager;
		return &EnemyManager;
	}
	
	int GetTarget(CEnemy* _enemy);

private:

	std::vector <const CEnemyPlanner*> EnemyPlannerBank;

	//単一性を保証(Singleton)//////////////////////////////////////////
		CEnemyPlanManager(){}
		CEnemyPlanManager(const CEnemyPlanManager& hoge);
		CEnemyPlanManager& operator=(const CEnemyPlanManager& hoge);
	///////////////////////////////////////////////////////////////////
};


////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////
