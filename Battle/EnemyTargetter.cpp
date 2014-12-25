#include "../Define.h"
#include "EnemyTargetter.h"
#include "Enemy.h"



const int CEnemyTargetter::ATTENTION_RATIO[MAX_PLAYER_NUM] = {4, 2, 1};


void CEnemyTargetter::CalcAttentionRank(int _attentionRank[]){

	for (int i=0; i<PLAYER_NUM; i++) {
		if (!Actor[i]->GetAlive()) continue;
		_attentionRank[i] = 0;
		for (int j=0; j<PLAYER_NUM; j++) {
			if (i != j && Actor[j]->GetAlive() && Attention[i] <= Attention[j]) {
				++_attentionRank[i];
			}
		} 
	}

}

int CEnemyTargetter_DEFAULT::GetTarget(const CEnemy* _enemy){

	//�S�����S���Ă���G���[��f���ďI��
	for (int i=0; i<PLAYER_NUM; i++){
		if (Actor[i]->GetAlive()) break;

		if (i==PLAYER_NUM-1) {
			ERRORDX("'%s':All_Players_Died", _enemy->GetName().c_str());
			return -1;
		}
	}

	//�A�e���V�����̏��ʂ����߂�
	int* attentionRank = new int[PLAYER_NUM];	//Attention�̏���0~
	CalcAttentionRank(attentionRank);

	//�A�e���V�����̏��ʂɉ����ă^�[�Q�b�g����
	int probability = 0;
	int target = -1;

	for (int i=0; i<PLAYER_NUM; i++){
		if (!Actor[i]->GetAlive() || Attention[i]==0) continue;		//���Ɏ��S���Ă���܂��̓A�e���V������0�̂Ƃ��̓^�[�Q�b�g�ɂ��Ȃ�
		probability += ATTENTION_RATIO[attentionRank[i]];
		if ((rand()%100)/(double)100 * probability < ATTENTION_RATIO[attentionRank[i]]){
			target = i;
		}
	}
	
	delete [] attentionRank;

	//�G���[�`�F�b�N
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

