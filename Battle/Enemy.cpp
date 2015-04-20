#include "../Define.h"
#include "Enemy.h"
#include "Species.h"
#include "../Main/TextBox.h"
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
					DEBUGDX("VisbleFalse:%s", Name.c_str());
				}
			//////////////////////////////////////////////////////

		}
		
		DrawGraph(Rect.Left+dx, Rect.Top+dy, Img, true);
		SetDrawBlendMode( DX_BLENDMODE_NOBLEND , 0 ) ;

		//HPBarやTimeGaugeの描画
			Draw_Sub(_dx, _dy);

		//AttentionCursorの描画
			AI.Draw(this);
	}
}


bool CEnemy::Plan(){
	
	//すべてActionに移動
	return true;

}

bool CEnemy::Action(){
	
	//行動の決定
		int actionNum = AI.GetPlan(this);
		if (actionNum>=0 && actionNum<(int)TrickList.size()){
			NowTrick = TrickList[actionNum];
		}else{
			ERRORDX("%s:actionNum<0 OR actionNum >= TrickList.size (do nothing):%d", GetName().c_str(), actionNum);
			return true;
		}

		if (NowTrick==NULL){
			Target = -1;
			ERRORDX("%s:NowTrick=NULL(do nothing)", GetName().c_str());
			return true;
		}

	//Tatgetの選択と行動
		char tmpcmd[256];
		Target = AI.GetTarget(this); 
		sprintf_s(tmpcmd, "@Damage(%d,%d,%d,NORMAL)", ActorIndex, Target, NowTrick);	//アドレスを渡している。intでキャストした方がいいのか？→いや、技名で渡せよ・・・(14/12/25)
		CmdList->Add(tmpcmd);
		/*
		switch(NowTrick->TargetType){
		case NowTrick->SINGLE:
			Target = AI.GetTarget(this); 
			sprintf_s(tmpcmd, "@Damage(%d,%d,%d,NORMAL)", ActorIndex, Target, NowTrick);	//アドレスを渡している。intでキャストした方がいいのか？→いや、技名で渡せよ・・・(14/12/25)
			CmdList->Add(tmpcmd);
			break;
		//case NowTrick->ALL:$
		default:	
			break;

		}*/

	//行動後処理
		NowTrick = NULL;
		Target = -1;
	
	return true;
}