#include "../Define.h"
#include "EnemyAI.h"
#include "Actor.h"
#include "Enemy.h"

int CEnemyAI::GetTarget(const CEnemy* _enemy){
	for (int i=0; i<PLAYER_NUM; i++){
		if (Actor[i]->GetAlive()) break;
		if (i==PLAYER_NUM-1) {
			ERRORDX("'%s':All_Players_Died", _enemy->GetName().c_str());
			return -1;
		}
	}
	
	int target = rand()%PLAYER_NUM;
	while(!Actor[target]->GetAlive()){
		target = rand()%PLAYER_NUM;
	}
	return target;
}
