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
			
			//�����ɃG�l�~�[���񂾂Ƃ��̃G�t�F�N�g����������///$
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

	EnemyPlanManager->MakePlan(Index, 50, 0, 1);	//������Hp50%�����E�ɁA�Z0���Z1

}


bool CEnemy::Plan(){
	/*//�e�X�g�p////////////////////////////////
	#ifdef DEBUG_MODE
		char tmp[64];
		sprintf_s(tmp, "%s��Plan", Name.c_str());
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

	//�Z�ȊO�̍s�����Ƃ�Ƃ��ɂ�EnemyPlannner.Plan�����̒l�i�Ⴆ�Ε����j��Ԃ�$
	
}

bool CEnemy::Action(){
	/*//�e�X�g�p////////////////////////////////
	#ifdef DEBUG_MODE
		char tmp[64];
		sprintf_s(tmp, "%s��Action", Name.c_str());
		(*B_TextBox_pp)->AddStock(tmp);
	#endif
	*///////////////////////////////////////////	

	if (NowTrick==NULL){
		Target = -1;
		ErrorDx("Error->CEnemey::Action->NowTrick=NULL(do nothing)");
		return true;
	}

	char tmpcmd[256];

	//Tatget�̑I���ɂ�Attention���g������$  Plan����Action�����ǂ�����Target�����߂�ׂ��H
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

	NowTrick = NULL;	//�g�p��͋��
	Target = -1;
	
	return true;
}