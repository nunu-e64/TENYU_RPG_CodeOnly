#include "Enemy.h"
#include "Species.h"
#include "LogWindow.h"
#include "../Main/TextBox.h"
#include "../Main/CmdList.h"



void CEnemy::Draw(int _dx, int _dy){
	int dx=_dx+Dx; int dy=_dy+Dy;

	switch(VisibleStatus) {
	case VISIBLE:
		SetDrawBright(255,255,255);
		break;
	case CHANGING:		
		{/////死亡演出//////////////////////////////////////
		static std::map<int, int> timeCount;
		if (timeCount.find(ActorIndex) == timeCount.end()) timeCount[ActorIndex] = 0;	//最初の一度だけ初期値代入
		timeCount[ActorIndex]++;
		
		SetDrawBlendMode( DX_BLENDMODE_ALPHA , 240-(timeCount[ActorIndex]*8)) ;
		if (timeCount[ActorIndex]==30){
			timeCount[ActorIndex] = 0;
			VisibleStatus = INVISIBLE;
		}
		}//////////////////////////////////////////////////////
		break;
	case INVISIBLE:
		return;
	}

	if (Visible) DrawGraph(Rect.Left+dx, Rect.Top+dy, Img, true);

	SetDrawBright(255,255,255);
	SetDrawBlendMode( DX_BLENDMODE_NOBLEND , 0 ) ;

	//HPBarやTimeGaugeの描画
		Draw_Sub(_dx, _dy);

	//AttentionCursorの描画
		AI.Draw(this);
	
}


bool CEnemy::Plan(){
	

	//行動の決定
		int actionNum = AI.GetPlan(this);
		if (actionNum>=0 && actionNum<(int)TrickList.size()){
			NowTrick = TrickList[actionNum];
			MaxTimeGauge = NowTrick->Time;
		}else{
			ERRORDX("%s:actionNum<0 OR actionNum >= TrickList.size (do nothing):%d", GetName().c_str(), actionNum);
			return true;
		}

		if (NowTrick==NULL){
			Target = -1;
			ERRORDX("%s:NowTrick=NULL(do nothing)", GetName().c_str());
			return true;
		}


	return true;
}

bool CEnemy::Action(){
	
	
	if (Status[WAIT] || NowTrick==NULL){	//待機を選択した場合
		Status[WAIT] = false;
	} else {
		//Tatgetの選択と行動
			Target = AI.GetTarget(this); 
			CmdList->Add("@Damage(%d,%d,%d,NORMAL)", ActorIndex, Target, NowTrick);
			LogWindow->Add("%sの%s！", Name.c_str(), NowTrick->Name);

		//行動後処理
			NowTrick = NULL;
			Target = -1;
	
	}

	return true;
}