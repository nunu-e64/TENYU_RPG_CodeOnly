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


bool CEnemy::Plan(){
	
	if (AI==NULL){
		WarningDx("Warning->%s has no AI", Name.c_str());
		return true;
	}

	int action_num = AI->GetPlan(this);

	if (action_num>=0 && action_num<(int)TrickList.size()){
		NowTrick = TrickList[action_num];

		return true;
	}else{
		ErrorDx("Error->CEnemy::Plan->action_num is bigger than TrickList.size (do nothing):%d", __FILE__, __LINE__, action_num);
		return true;
	}
	
}

bool CEnemy::Action(){

	if (NowTrick==NULL){
		Target = -1;
		ErrorDx("Error->CEnemey::Action->NowTrick=NULL(do nothing)");
		return true;
	}


	//Tatgetの選択にはAttentionを使うこと（じゃないとプレイヤーが戦略を立てにくい）$  Plan時かAction時かどっちでTargetを決めるべき？→Actionでしょ！
	char tmpcmd[256];
	switch(NowTrick->TargetType){
	case NowTrick->SINGLE:
		Target = AI->GetTarget(this);  //rand()%PLAYER_NUM;
		sprintf_s(tmpcmd, "@Damage(%d,%d,%d,NORMAL)", ActorIndex, Target, NowTrick);	//アドレスを渡している。intでキャストした方がいいのか？
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