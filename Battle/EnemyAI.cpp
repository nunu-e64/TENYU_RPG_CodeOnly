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

	//���o

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

	//����ł���A�e���V������0��
	for (int i=0; i<PLAYER_NUM; i++){
		if (Attention[i]!=0 && !Actor[i]->GetAlive()){
			SetAttention(i, 0);
		}
	}

	//�A�e���V�����̏��ʂ����߂�
	int* attentionRank = new int[PLAYER_NUM];	//Attention�̏���0~
	Targetter->CalcAttentionRank(attentionRank);


	//�A�e���V�����{�[�h�ƃ}�[�J�[�̕`��//////////////////////////////////////////////////

	CVector pos(_enemy->GetRect().Center() + CVector(-60, 50));
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawCenterGraph(pos.x, pos.y, AttentionBoardImg, true);

	const int YOHAKU_LEFTRIGHT = 21;	//Board�̍��[����`��̈捶�[�܂ł̋����i���E�Ώ́j
	const int YOHAKU_TOPBOTTOM = 7;		//Board�̏�[����`��̈��[�܂ł̋����i�㉺�Ώ́j
	CVector boardSize = GetGraphSize(AttentionBoardImg) - CVector(YOHAKU_LEFTRIGHT*2, YOHAKU_TOPBOTTOM*2);
	pos.x -= boardSize.x*0.5;
	pos.y += boardSize.y*0.5;		//AttentionBoard�̕`��̈�̍������W���擾

	/*for (int i=0; i<MAX_ATTENTION+1; i++){
		DrawLine((int)pos.x-3, (int)(pos.y - (i+0.5)*boardSize.y/(MAX_ATTENTION+1)), (int)(pos.x+boardSize.x+3), (int)(pos.y - (i+0.5)*boardSize.y/(MAX_ATTENTION+1)), GetColor(50,50,50)); 
	}*/	

	//int bright = 255;
	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, 230);
	SetDrawBlendMode( DX_BLENDMODE_NOBLEND , 0 ) ;
	for (int i=0; i<PLAYER_NUM; i++){
		//bright = choose(attentionRank[i]+1, 255, 200, 150);
		//SetDrawBright(bright, bright, bright);
		if (Actor[i]->GetAlive()) DrawCenterGraph(pos.x + (i+0.5)*boardSize.x/MAX_PLAYER_NUM, pos.y - (Attention[i]+0.5)*boardSize.y/(MAX_ATTENTION+1), AttentionMarkerImg[i], true);
	}
	SetDrawBright(255, 255, 255);
	SetDrawBlendMode( DX_BLENDMODE_NOBLEND , 0 ) ;
	delete [] attentionRank;
}