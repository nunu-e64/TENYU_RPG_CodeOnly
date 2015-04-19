#include "../Define.h"
#include "Player.h"
#include "Battle.h"



void CPlayer::CreateBattleMenu(){
	
	BattleMenu.Init(10, 360, 100, 110);

	BattleMenu.Create("待機");
	BattleMenu.Add("", "技");
	BattleMenu.Add("", "祈る");
	BattleMenu.Add("", "道具");
	BattleMenu.Add("", "逃亡");
	
	for (unsigned int i=0; i<TrickList.size(); i++){
		BattleMenu.Add("技", TrickList[i]->Name);
	}

	//テスト用/////////////////////////////////////$
	for (int i=0; i<7; i++){
		char tmp[32];	sprintf_s(tmp, "アイテム%d", i);
		BattleMenu.Add("道具", tmp);
	}	
	////////////////////////////////////////////////

}

void CPlayer::Draw(int _dx, int _dy){
	int dx=_dx+Dx; int dy=_dy+Dy;

	if (!Alive && Visible){
		static int timecount = 0;
		timecount++;

		SetDrawBright(250-(timecount*5),250-(timecount*5),250-(timecount*5));
		if (timecount==40){
			timecount=0;
			Visible = false;
		}
		//////////////////////////////////////////////////////

	}else if(!Visible){	//死んでる
		SetDrawBright(50,50,50);
	}else{ //生きてる
		SetDrawBright(255,255,255);
	}


	if (BattleMenu.Alive){
		static int timecount=0;
		if (timecount==120) timecount=0;
		DrawGraph(Rect.Left+dx, Rect.Top+dy+(int)(5*sin(++timecount*2*PI/120)), Img, true);
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

		if (Target != -1) {	//Targetが-1じゃない＝TargetMarker.DecideなどでTargetが選択済み
			return  (newplan=true);
		}

		if (BattleMenu.Alive){		
			CMenuNode* result = NULL;

			if (BattleMenu.Move(result)){

				if (mystrcmp(result->parent->label, "技")){
					unsigned int index = BattleMenu.GetIndex(result);
				
					if (index<TrickList.size()){
						NowTrick = TrickList[index];
						
						switch(NowTrick->TargetType){	//選んだ技の対象人数によって処理を変える$
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
	
				}else if (mystrcmp(result->label, "待機")){
					BattleMenu.Alive=false;
					return  (newplan=true);
				//else if("アイテム") ...
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
	//とりあえずSINGLEの時だけ実装　//ターゲットについてはここで管理。技の種別についてはCmdManagerおよびBattleで管理。
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


	//行動後処理
		NowTrick = NULL;
		Target = -1;

	return true;
}


