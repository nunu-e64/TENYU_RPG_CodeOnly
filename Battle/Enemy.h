////多重インクルード防止（インクルードガード）//
#ifndef ENEMY_H								////
#define ENEMY_H								////
////////////////////////////////////////////////

#include "Actor.h"

class CEnemy;
class CEnemyPlanner{
public:
	//void MakePlan();
	int Plan(CEnemy* _enemy);
};


class CEnemy : public CActor{	
	friend class CEnemyPlanner;
public:
	void Draw(int dx=0, int dy=0);

private:
	//メンバ関数
		bool Plan();
		bool Action();

	//メンバ変数
		CEnemyPlanner EnemyPlanner;
		int Attention[MAX_PLAYER];
};



////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////
