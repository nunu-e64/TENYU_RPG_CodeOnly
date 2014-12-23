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

	int* attentionIndex = new int[PLAYER_NUM];	//AttentionÇÃèáà 0~

	for (int i=0; i<PLAYER_NUM; i++) {
		if (!Actor[i]->GetAlive()) continue;
		attentionIndex[i] = 0;
		for (int j=0; j<PLAYER_NUM; j++) {
			if (i != j && Actor[j]->GetAlive() && Attention[i] <= Attention[j]) {
				++attentionIndex[i];
			}
		} 
	}

	int probability = 0;
	int target = -1;

	for (int i=0; i<PLAYER_NUM; i++){
		if (!Actor[i]->GetAlive()) continue;
		myLog("AttentionIndex[%d]:%d", i, attentionIndex[i]);
		probability += ATTENTION_RATIO[attentionIndex[i]];
		if ((rand()%100)/(double)100 * probability < ATTENTION_RATIO[attentionIndex[i]]){
			target = i;
		}
	}
	
	delete [] attentionIndex;

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

