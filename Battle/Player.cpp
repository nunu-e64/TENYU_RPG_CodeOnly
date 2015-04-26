#include "../Define.h"
#include "Player.h"
#include "Battle.h"


	
void CPlayer::SetExtraImg(CBImgBank* _bImgBank){
	_bImgBank->GetImg(MAGIC_COUNTER, MagicCounterImg, ARRAY_SIZE(MagicCounterImg));
}

void CPlayer::CreateBattleMenu(){
	
	BattleMenu.Init(10, 360, 100, 110);

	BattleMenu.Create("�ҋ@");
	BattleMenu.Add("", "�Z");
	BattleMenu.Add("", "�F��");
	BattleMenu.Add("", "�h��");
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

	if (Visible) {
		switch(VisibleStatus) {
		case VISIBLE:
			SetDrawBright(255,255,255);
			break;
		case CHANGING:		
			{///���S���o///////////////////////////////////////////////////
			static std::map<int, int> timeCount;
			if (timeCount.find(ActorIndex) == timeCount.end()) timeCount[ActorIndex] = 0;	//�ŏ��̈�x���������l���

			++timeCount[ActorIndex];

			SetDrawBright(250-(timeCount[ActorIndex]*5),250-(timeCount[ActorIndex]*5),250-(timeCount[ActorIndex]*5));
			if (timeCount[ActorIndex] == 40){
				timeCount[ActorIndex] = 0;
				VisibleStatus = INVISIBLE;
			}
			}//////////////////////////////////////////////////////
			break;
		case INVISIBLE:
			SetDrawBright(50,50,50);
			break;
		}
	}

	if (BattleMenu.Alive){
		static std::map<int, int> timeCount;
		if (timeCount.find(ActorIndex) == timeCount.end()) timeCount[ActorIndex] = 0;	//�ŏ��̈�x���������l���
		if (timeCount[ActorIndex]==120) timeCount[ActorIndex] = 0;
		++timeCount[ActorIndex];
		if (Visible) DrawGraph(Rect.Left+dx, Rect.Top+dy+(int)(5*sin(timeCount[ActorIndex]*2*PI/120)), Img, true);
	}else{
		if (Visible) DrawGraph(Rect.Left+dx, Rect.Top+dy, Img, true);
	}

	//�}�W�b�N�J�E���^�[�̕`��//////////////////////////////////
	if (VisibleStatus == VISIBLE){
		SetDrawBlendMode(DX_BLENDMODE_ADD, 200);
		int r = 40;
		double angle = PI/2 + 3*PI/(MAX_MAGIC_COUNT+2);
		for (int i=0; i<MAX_MAGIC_COUNT; i++) {
			if (i < MagicCount) SetDrawBright(255,255,255); 
			else SetDrawBright(100,100,100); 
			DrawCenterGraph(Rect.Center().x + r*cos(angle), Rect.Center().y + r*sin(angle), MagicCounterImg[(i<MagicCount?0:1)], true);
			angle += 2*PI/(MAX_MAGIC_COUNT+2);
		}
		SetDrawBlendMode(DX_BLENDMODE_ADD, 200);
	}
	////////////////////////////////////////////////////////////

	SetDrawBright(255,255,255);
	SetDrawBlendMode( DX_BLENDMODE_NOBLEND , 0 ) ;

	Draw_Sub(_dx, _dy);
	BattleMenu.Draw();
}

bool CPlayer::Plan(){
	static bool newPlan = true;
	
	if (newPlan){
		BattleMenu.Alive = true;
		BattleMenu.Cursor = BattleMenu.front;
		Status[MAGIC_DEFFENCE] = false;		
		newPlan = false;

	}else{

		if (Target != -1) {	//Target��-1����Ȃ���TargetMarker.Decide�Ȃǂ�Target���I���ς�
			if (NowTrick!=NULL) MagicCount -= NowTrick->Cost;	//���͏���
			return  (newPlan=true);
		}

		if (BattleMenu.Alive){		
			CMenuNode* result = NULL;

			if (BattleMenu.Move(result)){


				if (mystrcmp(result->parent->label, "�Z")){
					unsigned int index = BattleMenu.GetIndex(result);
				
					if (index<TrickList.size()){
						//���͊m�F
							if (TrickList[index]->Cost > MagicCount) {	//���͂͑���Ă��邩
								LogWindow->Add("���͂�����Ȃ��I");
								return false;
							} else {
								NowTrick = TrickList[index];
							}

							
						//�I�񂾋Z�̑Ώېl���ɂ���ď�����ς���$
							char tmpcmd[256]; 
							switch(NowTrick->TargetType){	
							case trick_tag::SINGLE:
								strcpy_s(tmpcmd,"@Target_Appear(ENEMY,0,false)");
								CmdList->Add(tmpcmd);
								break;
							case trick_tag::ALL:
								Target = PLAYER_NUM;
								break;
							case trick_tag::SINGLE_FRIEND:
								strcpy_s(tmpcmd,"@Target_Appear(PLAYER,0,false)");
								CmdList->Add(tmpcmd);
								break;
							default:
								WARNINGDX("NowTrick->TargetType->Not Found. %s", NowTrick->Name);
								return false; //(newPlan=true);
							}
						BattleMenu.Alive = false;

					}else{
						ErrorDx("Error->CPlayer::Plan->bigger action_num(do nothing):%d", __FILE__, __LINE__, index);
						NowTrick = NULL;
						return false; //(newPlan=true);
					}
	
				}else if (mystrcmp(result->label, "�ҋ@")){
					BattleMenu.Alive=false;
					return  (newPlan=true);

				}else if (mystrcmp(result->label, "�F��")){
					BattleMenu.Alive=false;
					return  (newPlan=true);

				}else if (mystrcmp(result->label, "�h��")){
					if (DEFFENCE_MC > MagicCount) {	//���͂͑���Ă��邩
						LogWindow->Add("���͂�����Ȃ��I");
						return false;
					} else {
						Status[MAGIC_DEFFENCE] = true;
						MagicCount-=DEFFENCE_MC;
						Mode = ACTION;		//����TimeForward��ACTION+1�����STAY�ɕς��
						BattleMenu.Alive=false;
						return  (newPlan=true);
					}
				
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
				char tmpcmd[256]; sprintf_s(tmpcmd, "@Target_Decide(%d)", ActorIndex);
				CmdList->Add(tmpcmd);
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
	
	if (NowTrick==NULL){	//�ҋ@��I�������ꍇ
		Target = -1;
	
		for (int i=0; i<ENEMY_NUM; i++) {
			char tmpcmd[256];
			sprintf_s(tmpcmd, "@Attention_Add(%d,%d,%d)", i, Index, (int)ATTENIOTN_WAITING);
			CmdList->Add(tmpcmd);
		}

		return true;
	}


	char tmpcmd[256];
	sprintf_s(tmpcmd, "@Damage(%d,%d,%d,NORMAL)", ActorIndex, Target, NowTrick);
	CmdList->Add(tmpcmd);

	char tmpMessage[256];
	sprintf_s(tmpMessage, "%s��%s�I", Name.c_str(), NowTrick->Name);
	LogWindow->Add(tmpMessage);


	//�s���㏈��
		NowTrick = NULL;
		Target = -1;

	return true;
}


