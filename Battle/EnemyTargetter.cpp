#include "../Define.h"
#include "EnemyTargetter.h"
#include "Enemy.h"


int CEnemyTargetter_DEFAULT::GetTarget(const CEnemy* _enemy){
	//4:2:1
	for (int i=0; i<PLAYER_NUM; i++){
		if (Actor[i]->GetAlive()) break;

		if (i==PLAYER_NUM-1) {
			ERRORDX("'%s':All_Players_Died", _enemy->GetName().c_str());
			return -1;
		}
	}

	for (int i=0; i<PLAYER_NUM; i++) {
		
	} 

	return 0;
}

int CEnemyTargetter_REVERSE::GetTarget(const CEnemy* _enemy){
	return 0;
}

