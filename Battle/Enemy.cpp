#include "../Define.h"
#include "Enemy.h"
#include "Battle.h"

void CEnemy::Draw(int cx, int cy, int dx, int dy){

	CVector imgsize = GetGraphSize(Img)*0.5;
	DrawGraph(cx-imgsize.x, cy-imgsize.y, Img, true);

	DrawBox(-1+cx-25+dx, -1+cy+imgsize.y+5+dy, 1+cx+25+dx, 1+cy+imgsize.y+15+dy, BLUE, true);
	DrawRectGraph(cx-25+dx, cy+imgsize.y+5+dy, 0, 0, (int)(50*Hp/MaxHp), 10, Img_hpbar,false,false);

	if (Mode==STAY||Mode==PREPARE) SetDrawBright(150,150,150);
	DrawBox(-1+cx-25+dx, -1+cy+imgsize.y+20+dy, 1+cx+25+dx, 1+cy+imgsize.y+30+dy, BLUE, true);
	DrawRectGraph(cx-25+dx, cy+imgsize.y+20+dy, 0, 0, (int)(50*TimeGauge/100), 10, ((Mode==STAY||Mode==PLAN)?Img_timebar[0]:Img_timebar[1]),false,false);
	
	SetDrawBright(255,255,255);
}

bool CEnemy::Plan(){
	//テスト用////////////////////////////////
		char tmp[64];
		sprintf_s(tmp, "Enemy%dのPlan", Index);
		(*B_TextBox_pp)->AddStock(tmp);
	//////////////////////////////////////////

	
	unsigned int action_num = EnemyPlanner.Plan(this);
	if (action_num<TrickList.size()){
		NowTrick = TrickList[action_num];


		return true;
	}else{
		ErrorDx("Error->CEnemy::Plan->bigger action_num(do nothing)", __FILE__, __LINE__);
		return true;
	}

	//技以外の行動をとるときにはEnemyPlannner.Planが他の値（例えば負数）を返す$
	
}

bool CEnemy::Action(){
	//テスト用////////////////////////////////
		char tmp[64];
		sprintf_s(tmp, "Enemy%dのAction", Index);
		(*B_TextBox_pp)->AddStock(tmp);
	//////////////////////////////////////////	

	if (NowTrick==NULL){
		Target = -1;
		ErrorDx("Error->CEnemey::Action->NowTrick=NULL(do nothing)");
		return true;
	}

	char tmpcmd[256];

	//Tatgetの選択にはAttentionを使うこと$  Plan時かAction時かどっちでTargetを決めるべき？
	switch(NowTrick->TargetType){
	case NowTrick->SINGLE:
		Target = rand()%MAX_PLAYER;
		sprintf_s(tmpcmd, "@Damage(%d,%d,%d,NORMAL)", ActorIndex, Target, NowTrick);
		CmdList->Add(tmpcmd);
		break;
	//case NowTrick->ALL:$
	default:	
		break;

	}

	NowTrick = NULL;	//使用後は空に
	Target = -1;
	
	return true;
}


int CEnemyPlanner::Plan(CEnemy* _enemy){

	//いろいろ複雑なAIを作りたい！	$
	//以下は仮実装

	if (_enemy->Hp*2 > _enemy->MaxHp){
		return 0;
	}else{
		return 1;
	}
}