#include "../Define.h"
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
		{/////���S���o//////////////////////////////////////
		static std::map<int, int> timeCount;
		if (timeCount.find(ActorIndex) == timeCount.end()) timeCount[ActorIndex] = 0;	//�ŏ��̈�x���������l���
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

	//HPBar��TimeGauge�̕`��
		Draw_Sub(_dx, _dy);

	//AttentionCursor�̕`��
		AI.Draw(this);
	
}


bool CEnemy::Plan(){
	
	//�s���̌���
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

	return true;
}

bool CEnemy::Action(){
	

	//Tatget�̑I���ƍs��
		char tmpcmd[256];
		Target = AI.GetTarget(this); 
		sprintf_s(tmpcmd, "@Damage(%d,%d,%d,NORMAL)", ActorIndex, Target, NowTrick);	//�A�h���X��n���Ă���Bint�ŃL���X�g�������������̂��H������A�Z���œn����E�E�E(14/12/25)
		CmdList->Add(tmpcmd);

		char tmpMessage[256];
		sprintf_s(tmpMessage, "%s��%s�I", Name.c_str(), NowTrick->Name);
		LogWindow->Add(tmpMessage);

	//�s���㏈��
		NowTrick = NULL;
		Target = -1;
	
		//DEBUGDX("EnemyActionFinished");

	return true;
}