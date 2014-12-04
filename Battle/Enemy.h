////多重インクルード防止（インクルードガード）//
#ifndef ENEMY_H								////
#define ENEMY_H								////
////////////////////////////////////////////////

#include "Actor.h"

class CEnemyPlanManager;
class CEnemySpecies;

class CEnemy : public CActor, public CEnemySpecies{
public:
	
	CEnemy(){};
	CEnemy(const CEnemySpecies& obj) : CEnemySpecies(obj){};

	void SetEnemyPlanManager(CEnemyPlanManager* _enemyPlanManager){
		EnemyPlanManager = _enemyPlanManager;
	}

	void Draw(int _dx=0, int _dy=0);
	void MakePlan();

	//CEnemyPlannner AI;
	
private:
	//メンバ関数
		bool Plan();
		bool Action();

	//メンバ変数
		CEnemyPlanManager* EnemyPlanManager;
		int Attention[MAX_PLAYER_NUM];
};



////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////
