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
			
			//�����ɃG�l�~�[���񂾂Ƃ��̃G�t�F�N�g����������///$
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

		//HPBar��TimeGauge�̕`��
			Draw_Sub(_dx, _dy);

		//AttentionCursor�̕`��
			AI.Draw(this);
	}
}


bool CEnemy::Plan(){
	
	//���ׂ�Action�Ɉړ�
	return true;

}

bool CEnemy::Action(){
	
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

	//Tatget�̑I���ƍs��
		char tmpcmd[256];
		Target = AI.GetTarget(this); 
		sprintf_s(tmpcmd, "@Damage(%d,%d,%d,NORMAL)", ActorIndex, Target, NowTrick);	//�A�h���X��n���Ă���Bint�ŃL���X�g�������������̂��H������A�Z���œn����E�E�E(14/12/25)
		CmdList->Add(tmpcmd);
		/*
		switch(NowTrick->TargetType){
		case NowTrick->SINGLE:
			Target = AI.GetTarget(this); 
			sprintf_s(tmpcmd, "@Damage(%d,%d,%d,NORMAL)", ActorIndex, Target, NowTrick);	//�A�h���X��n���Ă���Bint�ŃL���X�g�������������̂��H������A�Z���œn����E�E�E(14/12/25)
			CmdList->Add(tmpcmd);
			break;
		//case NowTrick->ALL:$
		default:	
			break;

		}*/

	//�s���㏈��
		NowTrick = NULL;
		Target = -1;
	
	return true;
}