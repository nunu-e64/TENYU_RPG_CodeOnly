#include "../Define.h"
#include "EnemyTargetter.h"
#include "Enemy.h"



const int CEnemyTargetter::ATTENTION_RATIO[MAX_PLAYER_NUM] = {4, 2, 1};


int CEnemyTargetter_DEFAULT::GetTarget(const CEnemy* _enemy){

	for (int i=0; i<PLAYER_NUM; i++){
		if (Actor[i]->GetAlive()) break;

		if (i==PLAYER_NUM-1) {
			ERRORDX("'%s':All_Players_Died", _enemy->GetName().c_str());
			return -1;
		}
	}

	int* rank = new int[PLAYER_NUM];	//hî‘ñ⁄Ç…AttentionÇÃçÇÇ¢PlayerIndex
	int tmp;
	for (int h=0; h<PLAYER_NUM; h++) {	
		rank[h] = h;
	}
	for (int h=0; h<PLAYER_NUM-1; h++) {
		for (int i=h+1; i<PLAYER_NUM; i++) {
			if ((!Actor[i]->GetAlive()) || (Attention[i] > Attention[rank[h]] && Actor[i]->GetAlive())) {
				tmp = rank[h];
				rank[h] = rank[i];
				rank[i] = tmp;
			}
		} 
	}

	int probability = 0;
	int target = -1;
	int tmpRatio;

	for (unsigned int i=0; i<PLAYER_NUM; i++){
		tmpRatio = (Actor[i]->GetAlive()? ATTENTION_RATIO[rank[i]]: 0);
		probability += tmpRatio;
		if ((rand()%100)/(double)100 * probability < tmpRatio){
			target = i;
		}
	}
	
	delete [] rank;

	if (target < 0) {
		ERRORDX("%s:Target < 0 :%d", _enemy->GetName().c_str(), target);
		return -1;
	} else if (target>=PLAYER_NUM) {
		ERRORDX("%s:Target >= PLAYER_NUM :%d", _enemy->GetName().c_str(), target);
		return -1;
	}

	return target;
}

int CEnemyTargetter_REVERSE::GetTarget(const CEnemy* _enemy){
	return 0;
}

