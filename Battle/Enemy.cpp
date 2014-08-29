#include "../Define.h"
#include "Enemy.h"
#include "Battle.h"
#include "EnemyPlanManager.h"
#include "EnemySpecies.h"




void CEnemy::Draw(int dx, int dy){

	if (Visible){
		if (Alive==false){
			static int timecount = 0;
			timecount++;

			SetDrawBlendMode( DX_BLENDMODE_ALPHA , 240-(timecount*8)) ;

			if (timecount==30){
				timecount=0;
				Visible = false;
			}
		}

		DrawGraph(Rect.Left, Rect.Top, Img, true);
		SetDrawBlendMode( DX_BLENDMODE_NOBLEND , 0 ) ;


		//HpBar
		DrawBox(-1+Rect.Center().x-25+dx, -1+Rect.Bottom+5+dy, 1+Rect.Center().x+25+dx, 1+Rect.Bottom+15+dy, BLUE, true);
		DrawRectGraph(Rect.Center().x-25+dx, Rect.Bottom+5+dy, 0, 0, (int)(50*OldHp/MaxHp), 10, Img_hpbar,false,false);

		//TimeBar
		if (Mode==STAY||Mode==PREPARE) SetDrawBright(150,150,150);
		DrawBox(-1+Rect.Center().x-25+dx, -1+Rect.Bottom+20+dy, 1+Rect.Center().x+25+dx, 1+Rect.Bottom+30+dy, BLUE, true);
		DrawRectGraph(Rect.Center().x-25+dx, Rect.Bottom+20+dy, 0, 0, (int)(50*TimeGauge/100), 10, ((Mode==STAY||Mode==PLAN)?Img_timebar[0]:Img_timebar[1]),false,false);

		SetDrawBright(255,255,255);

		//OldHp��Hp�̃M���b�v�𖄂߂�
		if (OldHp>Hp) OldHp--;
		else if (OldHp<Hp) OldHp++;
	}
}

void CEnemy::MakePlan(){

	EnemyPlanManager->MakePlan(Index, 50, 0, 1);	//������Hp50%�����E�ɁA�Z0���Z1

}


bool CEnemy::Plan(){
	//�e�X�g�p////////////////////////////////
		char tmp[64];
		sprintf_s(tmp, "Enemy%d��Plan", Index);
		(*B_TextBox_pp)->AddStock(tmp);
	//////////////////////////////////////////

	
	unsigned int action_num = EnemyPlanManager->Plan(this);

	if (action_num<TrickList.size()){
		NowTrick = TrickList[action_num];


		return true;
	}else{
		ErrorDx("Error->CEnemy::Plan->bigger action_num(do nothing)", __FILE__, __LINE__);
		return true;
	}

	//�Z�ȊO�̍s�����Ƃ�Ƃ��ɂ�EnemyPlannner.Plan�����̒l�i�Ⴆ�Ε����j��Ԃ�$
	
}

bool CEnemy::Action(){
	//�e�X�g�p////////////////////////////////
		char tmp[64];
		sprintf_s(tmp, "Enemy%d��Action", Index);
		(*B_TextBox_pp)->AddStock(tmp);
	//////////////////////////////////////////	

	if (NowTrick==NULL){
		Target = -1;
		ErrorDx("Error->CEnemey::Action->NowTrick=NULL(do nothing)");
		return true;
	}

	char tmpcmd[256];

	//Tatget�̑I���ɂ�Attention���g������$  Plan����Action�����ǂ�����Target�����߂�ׂ��H
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

	NowTrick = NULL;	//�g�p��͋��
	Target = -1;
	
	return true;
}