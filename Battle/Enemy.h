////多重インクルード防止（インクルードガード）//
#ifndef ENEMY_H								////
#define ENEMY_H								////
////////////////////////////////////////////////

#include "Actor.h"

class CEnemyPlanManager;

class CEnemy : public CActor{	
public:
	
	void SetEnemyPlanManager(CEnemyPlanManager* _enemyPlanManager){
		EnemyPlanManager = _enemyPlanManager;
	}

	void Draw(int dx=0, int dy=0);
	void MakePlan();	
	
private:
	//メンバ関数
		bool Plan();
		bool Action();

	//メンバ変数
		CEnemyPlanManager* EnemyPlanManager;
		int Attention[MAX_PLAYER];
};



////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////
