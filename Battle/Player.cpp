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
	for (int i=0; i<7; i++){
		char tmp[32];	sprintf_s(tmp, "�A�C�e��%d", i);
		BattleMenu.Add("����", tmp);
	}	
	////////////////////////////////////////////////

}

void CPlayer::Draw(int _dx, int _dy){
	int dx=_dx+Dx; int dy=_dy+Dy;

	if (!Alive && Visible){
		
		static std::map<int, int> timeCount;
		if (timeCount.find(ActorIndex) == timeCount.end()) timeCount[ActorIndex] = 0;	//�ŏ��̈�x���������l���

		++timeCount[ActorIndex];

		SetDrawBright(250-(timeCount[ActorIndex]*5),250-(timeCount[ActorIndex]*5),250-(timeCount[ActorIndex]*5));
		if (timeCount[ActorIndex] == 40){
			timeCount[ActorIndex] = 0;
			Visible = false;
		}
		//////////////////////////////////////////////////////

	}else if(!Visible){	//����ł�
		SetDrawBright(50,50,50);
	}else{ //�����Ă�
		SetDrawBright(255,255,255);
	}


	if (BattleMenu.Alive){
		static std::map<int, int> timeCount;
		if (timeCount.find(ActorIndex) == timeCount.end()) timeCount[ActorIndex] = 0;	//�ŏ��̈�x���������l���
		if (timeCount[ActorIndex]==120) timeCount[ActorIndex] = 0;
		++timeCount[ActorIndex];
		DrawGraph(Rect.Left+dx, Rect.Top+dy+(int)(5*sin(timeCount[ActorIndex]*2*PI/120)), Img, true);
	}else{
		DrawGraph(Rect.Left+dx, Rect.Top+dy, Img, true);
	}

	Draw_Sub(_dx, _dy);
	BattleMenu.Draw();
}

bool CPlayer::Plan(){
	static bool newplan = true;
	
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

				if (mystrcmp(result->parent->label, "�Z")){
					unsigned int index = BattleMenu.GetIndex(result);
				
					if (index<TrickList.size()){
						NowTrick = TrickList[index];
						char tmpcmd[256]; 

						switch(NowTrick->TargetType){	//�I�񂾋Z�̑Ώېl���ɂ���ď�����ς���$
						case trick_tag::SINGLE:
							strcpy_s(tmpcmd,"@Target_Appear(ENEMY,0)");
							CmdList->Add(tmpcmd);
							break;
						case trick_tag::ALL:
							Target = PLAYER_NUM;
							break;
						case trick_tag::SINGLE_FRIEND:
							strcpy_s(tmpcmd,"@Target_Appear(PLAYER,0)");
							CmdList->Add(tmpcmd);
							break;
						default:
							WARNINGDX("NowTrick->TargetType->Not Found. %s", NowTrick->Name);
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
	
	if (NowTrick==NULL){
		Target = -1;
		//DebugDx("Error->CPlayer::Action->NowTrick=NULL(do nothing)_Player%d",Index);
	
		for (int i=0; i<ENEMY_NUM; i++) {
			char tmpcmd[256];
			sprintf_s(tmpcmd, "@Attention_Add(%d,%d,%d)", i, Index, (int)ATTENIOTN_WAITING);
			CmdList->Add(tmpcmd);
		}

		return true;
	}


	/*
	//�Ƃ肠����SINGLE�̎����������@//�^�[�Q�b�g�ɂ��Ă͂����ŊǗ��B�Z�̎�ʂɂ��Ă�CmdManager�����Battle�ŊǗ��B
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
	*/

		char tmpcmd[256];
		sprintf_s(tmpcmd, "@Damage(%d,%d,%d,NORMAL)", ActorIndex, Target, NowTrick);
		CmdList->Add(tmpcmd);


	//�s���㏈��
		NowTrick = NULL;
		Target = -1;

	return true;
}


