#include "../Define.h"
#include "EnemyAI.h"
#include "Actor.h"
#include "Enemy.h"


int CEnemyAI::AttentionCursor[MAX_PLAYER_NUM] = {0};


int CEnemyAI::GetPlan(const CEnemy* _enemy) { 
	return Planner->GetPlan(_enemy); 
}

int CEnemyAI::GetTarget(const CEnemy* _enemy){
	return Targetter->GetTarget(_enemy);
	
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

bool CEnemyAI::AddRandomPlanSet(const unsigned int _index, std::vector<std::pair<int, int> > _planList, bool _clear){

	if (_clear) RandomPlanSet.clear();

	if (RandomPlanSet.find(_index)==RandomPlanSet.end()){
		RandomPlanSet[_index] = _planList;
		return true;

	}else{
		ErrorDx("Error->SetRandomPlanSet->Already Existed Key(don't override):%d", (int)_index);
		return  false;
	}
}

void CEnemyAI::SetAttentionCursorImage(int _index, int _img){
	if (_index<MAX_PLAYER_NUM && _index>=0) {
		CEnemyAI::AttentionCursor[_index] = _img;
	} else {
		ErrorDx("_index Error:%d cf:MAX_PLAYER_NUM:%d", _index, MAX_PLAYER_NUM);
	}
}


void CEnemyAI::Draw(const CEnemy* _enemy){
	
	//アテンションマーカーの描画//////////////////////////////////////////////////

	int* attentionRank = new int[PLAYER_NUM];	//Attentionの順位
	Targetter->CalcAttentionRank(attentionRank);

	CVector center = _enemy->GetRect().Center();
	const int KANKAKU = 35;
	for (int i=0; i<PLAYER_NUM; i++){
		if (Actor[i]->GetAlive()){
			double x = center.x + (i - (PLAYER_NUM-1)/(double)2) * KANKAKU;
			if (DrawRotaGraph((int)x, (int)center.y, ExtRate[attentionRank[i]], atan2(Actor[i]->GetRect().Center().y-center.y, Actor[i]->GetRect().Center().x-x), AttentionCursor[i], true, false) == -1){
				ERRORDX("DrawError");
			}
		}
	}

	delete [] attentionRank;

}