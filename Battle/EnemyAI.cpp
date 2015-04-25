#include "../Define.h"
#include "EnemyAI.h"
#include "Actor.h"
#include "Enemy.h"


int CEnemyAI::AttentionMarkerImg[MAX_PLAYER_NUM] = {0};
int CEnemyAI::AttentionBoardImg = 0;
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

void CEnemyAI::AddAttention(int _playerIndex, int _value){
	Attention[_playerIndex] = between(0, MAX_ATTENTION, Attention[_playerIndex]+_value); 

	//演出

	//myLogf("Attention_P", "EnemyAI:%d", Attention);


}
void CEnemyAI::SetAttention(int _playerIndex, int _value){
	AddAttention(_playerIndex, _value - Attention[_playerIndex]); 	
}

void CEnemyAI::SetAttentionImg(int* _markerImg, int _boardImg){
	if (_markerImg!=NULL) {
		for (int i=0; i<MAX_PLAYER_NUM; i++){
			AttentionMarkerImg[i] = _markerImg[i];
		}
	}

	AttentionBoardImg = _boardImg;
}

void CEnemyAI::Draw(const CEnemy* _enemy){

	//死んでたらアテンションは0に
	for (int i=0; i<PLAYER_NUM; i++){
		if (!Actor[i]->GetAlive() && Attention[i]!=0){
			SetAttention(i, 0);
		}
	}

	//アテンションの順位を求める
	int* attentionRank = new int[PLAYER_NUM];	//Attentionの順位0~
	Targetter->CalcAttentionRank(attentionRank);


	//アテンションマーカーの描画//////////////////////////////////////////////////

	CVector pos(_enemy->GetRect().Center() + CVector(-50, 50));
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawCenterGraph(pos.x, pos.y, AttentionBoardImg, false);

	int boardHeight = (int)GetGraphSize(AttentionBoardImg).y;
	pos.y += boardHeight/2;		//AttentionBoardの中央下端座標を取得
	CVector markerSize = GetGraphSize(AttentionMarkerImg[0]);

	for (int i=0; i<PLAYER_NUM; i++){

		//アテンションの高い順位に濃くマーカーが描かれる
		
		SetDrawBlendMode( DX_BLENDMODE_NOBLEND , 0 ) ;
		SetDrawBlendMode( DX_BLENDMODE_ALPHA , (Attention[i]==0? 100: choose(attentionRank[i]+1, 220, 160, 100)));
		double exRate = chooseT(attentionRank[i]+1, 1.0, 0.6, 0.3);
		DrawRotaGraph3((int)pos.x, (int)(pos.y - Attention[i]*boardHeight/MAX_ATTENTION), (int)markerSize.x/2, (int)markerSize.y/2, exRate, 1, 0, AttentionMarkerImg[i], false);
	}

	SetDrawBlendMode( DX_BLENDMODE_NOBLEND , 0 ) ;
	delete [] attentionRank;
}