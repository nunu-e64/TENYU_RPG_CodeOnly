#include "../Define.h"
#include "EnemyTargetter.h"
#include "Enemy.h"



const int CEnemyTargetter::ATTENTION_RATIO[MAX_PLAYER_NUM] = {4, 2, 1};


void CEnemyTargetter::CalcAttentionRank(int _attentionRank[]){

	//myLogf("attention", "[0]%d, [1]%d, [2]%d", Attention[0], Attention[1], Attention[2]);


	for (int i=0; i<PLAYER_NUM; i++) {
		_attentionRank[i] = 0;

		for (int j=0; j<PLAYER_NUM; j++) {
			if ((i != j) && (Attention[i] <= Attention[j])) {
				++_attentionRank[i];
			}
		} 
	}

	return;//$

	//上に詰める
	int i = 0;
	while(_attentionRank[i] != 0){
		if (i == PLAYER_NUM-1) {
			myLog("うえつめ");
			i = 0;
			for (int j=0; j<PLAYER_NUM; j++) {
				++_attentionRank[j];
			}
		} else {
			++i;
		}
	}
}

int CEnemyTargetter_DEFAULT::GetTarget(const CEnemy* _enemy){

	//全員死亡してたらエラーを吐いて終了
	for (int i=0; i<PLAYER_NUM; i++){
		if (Actor[i]->GetAlive()) break;

		if (i==PLAYER_NUM-1) {
			ERRORDX("'%s':All_Players_Died", _enemy->GetName().c_str());
			return -1;
		}
	}

	//アテンションの順位を求める
	int* attentionRank = new int[PLAYER_NUM];	//Attentionの順位0~
	CalcAttentionRank(attentionRank);

	//アテンションの順位に応じてターゲット決定
	int probability = 0;
	int target = -1;

	for (int i=0; i<PLAYER_NUM; i++){
		if (!Actor[i]->GetAlive() || Attention[i]==0) continue;		//既に死亡しているまたはアテンションが0のときはターゲットにしない
		probability += ATTENTION_RATIO[attentionRank[i]];
		if ((rand()%100)/(double)100 * probability < ATTENTION_RATIO[attentionRank[i]]){
			target = i;
		}
	}
	
	delete [] attentionRank;

	//エラーチェック
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

