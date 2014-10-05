////多重インクルード防止（インクルードガード）//
#ifndef ENEMY_H								////
#define ENEMY_H								////
////////////////////////////////////////////////

#include "Actor.h"

class CEnemyPlanManager;
class CEnemySpecies;

//ゆくゆくはCActorとCEnemySpeciesから多重継承する
//戦闘情報はActorが、取得ゴールドや経験値情報はEnemySpeciesが持っているため。

class CEnemy : public CActor, public CEnemySpecies{
public:
	
	CEnemy(){};
	CEnemy(const CSpecies& _species) : CActor(_species){};
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
