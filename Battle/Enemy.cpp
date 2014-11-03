#include "../Define.h"
#include "Enemy.h"
#include "EnemyPlanManager.h"
#include "Species.h"
#include "../Main/TextBox.h"
//#include "../Main/TextWrap.h"
#include "../Main/CmdList.h"



void CEnemy::Draw(int _dx, int _dy){
	int dx=_dx+Dx; int dy=_dy+Dy;

	if (Visible){
		if (!Alive){

			static int timecount = 0;
			timecount++;
			
			//ここにエネミー死んだときのエフェクト処理を書く///$
				SetDrawBlendMode( DX_BLENDMODE_ALPHA , 240-(timecount*8)) ;

				if (timecount==30){
					timecount=0;
					Visible = false;
				}
			//////////////////////////////////////////////////////

		}
		
		DrawGraph(Rect.Left+dx, Rect.Top+dy, Img, true);
		SetDrawBlendMode( DX_BLENDMODE_NOBLEND , 0 ) ;

		Draw_Sub(_dx, _dy);
	}
}

void CEnemy::MakePlan(){

	EnemyPlanManager->MakePlan(Index, 50, 0, 1);	//自分のHp50%を境界に、技0→技1

}


bool CEnemy::Plan(){
	/*//テスト用////////////////////////////////
	#ifdef DEBUG_MODE
		char tmp[64];
		sprintf_s(tmp, "%sのPlan", Name.c_str());
		(*B_TextBox_pp)->AddStock(tmp);
	#endif
	*///////////////////////////////////////////

	
	unsigned int action_num = EnemyPlanManager->Plan(this);

	if (action_num<TrickList.size()){
		NowTrick = TrickList[action_num];


		return true;
	}else{
		ErrorDx("Error->CEnemy::Plan->action_num is bigger than TrickList.size (do nothing):%d", __FILE__, __LINE__, action_num);
		return true;
	}

	//技以外の行動をとるときにはEnemyPlannner.Planが他の値（例えば負数）を返す$
	
}

bool CEnemy::Action(){
	/*//テスト用////////////////////////////////
	#ifdef DEBUG_MODE
		char tmp[64];
		sprintf_s(tmp, "%sのAction", Name.c_str());
		(*B_TextBox_pp)->AddStock(tmp);
	#endif
	*///////////////////////////////////////////	

	if (NowTrick==NULL){
		Target = -1;
		ErrorDx("Error->CEnemey::Action->NowTrick=NULL(do nothing)");
		return true;
	}

	char tmpcmd[256];

	//Tatgetの選択にはAttentionを使うこと$  Plan時かAction時かどっちでTargetを決めるべき？
	switch(NowTrick->TargetType){
	case NowTrick->SINGLE:
		Target = rand()%PLAYER_NUM;
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