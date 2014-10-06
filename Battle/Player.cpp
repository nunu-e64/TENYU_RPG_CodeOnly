#include "../Define.h"
#include "Player.h"
#include "Battle.h"



void CPlayer::CreateBattleMenu(){
	
	BattleMenu.Init(10, 360, 100, 110);

	BattleMenu.Create("�ҋ@");
	BattleMenu.Add("", "�Z");
	BattleMenu.Add("", "�F��");
	BattleMenu.Add("", "����");
	BattleMenu.Add("", "���S");
	
	for (unsigned int i=0; i<TrickList.size(); i++){
		BattleMenu.Add("�Z", TrickList[i]->Name);
	}

	//�e�X�g�p/////////////////////////////////////$
	for (int i=0; i<10; i++){
		char tmp[32];	sprintf_s(tmp, "�A�C�e��%d", i);
		BattleMenu.Add("����", tmp);
	}	
	////////////////////////////////////////////////

}

void CPlayer::Draw(int dx, int dy){

	if (!Alive && Visible){
		static int timecount = 0;
		timecount++;

		//�����Ƀv���C���[�����񂾂Ƃ��̃G�t�F�N�g����������///$
			/*if (timecount%11<5){
				SetDrawBright(50,50,50);
			}*/
		SetDrawBlendMode( DX_BLENDMODE_ALPHA , 250-(timecount*5)) ;
		if (timecount==40){
			timecount=0;
			Visible = false;
		}
		//////////////////////////////////////////////////////

	}else if(!Visible){	//����ł�
		SetDrawBright(50,50,50);
	}else{
		SetDrawBright(255,255,255);
	}

	DrawGraph(Rect.Left+dx, Rect.Top+dy, Img, true);

	Draw_Sub(dx, dy);
	BattleMenu.Draw();	
}

bool CPlayer::Plan(){
	static bool newplan = true;
	
	//�e�X�g�p////////////////////////////////
	if (newplan){
		char tmp[64];
		sprintf_s(tmp, "Player%d��Plan", Index);
		//(*B_TextBox_pp)->AddStock(tmp);
	}
	//////////////////////////////////////////


	if (newplan){
		BattleMenu.Alive = true;
		BattleMenu.Cursor = BattleMenu.front;
		newplan = false;

	}else{

		if (Target != -1) {	//Target��-1����Ȃ���TargetMarker.Decide�Ȃǂ�Target���I���ς�
			return  (newplan=true);
		}

		if (BattleMenu.Alive){		
			CMenuNode* result = NULL;

			if (BattleMenu.Move(result)){
				DebugDx("MenuResult:%s", result->label);

				if (mystrcmp(result->parent->label, "�Z")){
					unsigned int index = BattleMenu.GetIndex(result);
				
					if (index<TrickList.size()){
						NowTrick = TrickList[index];
						
						switch(NowTrick->TargetType){	//�I�񂾋Z�̑Ώېl���ɂ���ď�����ς���$
						case NowTrick->SINGLE:
							char tmpcmd[256]; strcpy_s(tmpcmd,"@Target_Appear(ENEMY,0)");
							CmdList->Add(tmpcmd);
							break;
						//case NowTrick->ALL:
						default:					
							return (newplan=true);
						}
						BattleMenu.Alive=false;

					}else{
						ErrorDx("Error->CPlayer::Plan->bigger action_num(do nothing):%d", __FILE__, __LINE__, index);
						NowTrick = NULL;
						return (newplan=true);
					}
	
				}else if (mystrcmp(result->label, "�ҋ@")){
					BattleMenu.Alive=false;
					return  (newplan=true);
				//else if("�A�C�e��") ...
				}
			}
		}else{

			if (CheckHitKeyDown(KEY_INPUT_RIGHT)){
				char* tmpcmd = "@Target_Move(RIGHT)";
				CmdList->Add(tmpcmd);
			}else if (CheckHitKeyDown(KEY_INPUT_LEFT)){
				char* tmpcmd = "@Target_Move(LEFT)";
				CmdList->Add(tmpcmd);
			}else if (CheckHitKeyDown(KEY_INPUT_OK)){
				char tmpcmd[256]; sprintf_s(tmpcmd, "@Target_Decide(%d,false)", ActorIndex);
				CmdList->Add(tmpcmd);
				return false;
			}else if (CheckHitKeyDown(KEY_INPUT_CANCEL)){
				char* tmpcmd = "@Target_Disappear";
				CmdList->Add(tmpcmd);
				NowTrick = NULL;
				BattleMenu.Alive = true;
			}
		}
	}

	return false;
}

bool CPlayer::Action(){
	//�e�X�g�p////////////////////////////////
	char tmp[64];
	sprintf_s(tmp, "Player%d��Action", Index);
	//(*B_TextBox_pp)->AddStock(tmp);
	//////////////////////////////////////////
	
	if (NowTrick==NULL){
		Target = -1;
		ErrorDx("Error->CPlayer::Action->NowTrick=NULL(do nothing)_Player%d",Index);
		return true;
	}

	
	//�Ƃ肠����SINGLE�̎���������
	switch (NowTrick->TargetType){
	case NowTrick->SINGLE:
		char tmpcmd[256];
		sprintf_s(tmpcmd, "@Damage(%d,%d,%d,NORMAL)", ActorIndex, Target, NowTrick);
		CmdList->Add(tmpcmd);
		break;

	//case ALL:
	default:
		break;
	}

	NowTrick = NULL;	//�g�p��͋��
	Target = -1;
	return true;
}


