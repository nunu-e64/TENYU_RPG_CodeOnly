#include "../Define.h"
#include "EnemyAI.h"
#include "Actor.h"
#include "Enemy.h"


int CEnemyAI::AttentionMarkerImg = 0;
CVector CEnemyAI::AttentionMarkerImgSize;


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

void CEnemyAI::AddAttention(int _playerNum, attention_tag _value){
	AddAttention(_playerNum, (int)_value); 
}
void CEnemyAI::AddAttention(int _playerNum, int _value){
	Attention[_playerNum] = between(0, MAX_ATTENTION, Attention[_playerNum]+_value); 

	//演出

}
void CEnemyAI::SetAttention(int _playerNum, int _value){
	AddAttention(_playerNum, _value - Attention[_playerNum]); 	
}

void CEnemyAI::SetAttentionMarkerImage(int _img){
	AttentionMarkerImg = _img;
	int x; int y;
	GetGraphSize(AttentionMarkerImg, &x, &y); 
	AttentionMarkerImgSize = CVector(x/(MAX_ATTENTION+2), y/MAX_PLAYER_NUM);
}

void CEnemyAI::Draw(const CEnemy* _enemy){
	
	//アテンションマーカーの描画//////////////////////////////////////////////////
	SetDrawBlendMode( DX_BLENDMODE_ALPHA , 150) ;

	CVector pos(_enemy->GetRect().Center().x-AttentionMarkerImgSize.x/2, max(0, _enemy->GetRect().Top-AttentionMarkerImgSize.y/2));
	const int KANKAKU = 35;
	
	for (int i=0; i<PLAYER_NUM; i++){
		if (!Actor[i]->GetAlive()){
			SetAttention(i, 0);
		}
		double x = pos.x + (i - (PLAYER_NUM-1)/(double)2) * KANKAKU;
		DrawRectGraph((int)x, (int)pos.y, (int)(Attention[i] * AttentionMarkerImgSize.x), (int)(i * AttentionMarkerImgSize.y), (int)AttentionMarkerImgSize.x, (int)AttentionMarkerImgSize.y, AttentionMarkerImg, true, false);
	}

	SetDrawBlendMode( DX_BLENDMODE_NOBLEND , 0 ) ;

}