#include "../Define.h"
#include "EnemyPlanManager.h"
#include "Enemy.h"

int CEnemyPlanManager::GetTarget(CEnemy* _enemy){

	int target=-1;

	target = rand()%3;
	
	return target;
}

