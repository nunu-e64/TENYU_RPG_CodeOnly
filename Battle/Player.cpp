#include "../Define.h"
#include "Player.h"
#include "Battle.h"



void CPlayer::Init(){
	
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
	for (int i=0; i<10; i++){
		char tmp[32];	sprintf_s(tmp, "アイテム%d", i);
		BattleMenu.Add("道具", tmp);
	}	
	////////////////////////////////////////////////

}

void CPlayer::Draw(int cx, int cy, int dx, int dy){

	CVector imgsize = GetGraphSize(Img)*0.5;
	DrawGraph(cx-imgsize.x, cy-imgsize.y, Img, true);

	DrawBox(-1+cx-25+dx, -1+cy+imgsize.y+5+dy, 1+cx+25+dx, 1+cy+imgsize.y+15+dy, BLUE, true);
	DrawRectGraph(cx-25+dx, cy+imgsize.y+5+dy, 0, 0, (int)(50*Hp/MaxHp), 10, Img_hpbar,false,false);

	if (Mode==STAY||Mode==PREPARE) SetDrawBright(150,150,150);
	DrawBox(-1+cx-25+dx, -1+cy+imgsize.y+20+dy, 1+cx+25+dx, 1+cy+imgsize.y+30+dy, BLUE, true);
	DrawRectGraph(cx-25+dx, cy+imgsize.y+20+dy, 0, 0, (int)(50*TimeGauge/100), 10, ((Mode==STAY||Mode==PLAN)?Img_timebar[0]:Img_timebar[1]),false,false);
	SetDrawBright(255,255,255);

	BattleMenu.Draw();
}

bool CPlayer::Plan(){
	static bool newplan = true;
	
	//テスト用////////////////////////////////
	if (newplan){
		char tmp[64];
		sprintf_s(tmp, "Player%dのPlan", Index);
		(*B_TextBox_pp)->AddStock(tmp);
	}
	//////////////////////////////////////////


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
				DebugDx("MenuResult:%s", result->label);

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
	//テスト用////////////////////////////////
	char tmp[64];
	sprintf_s(tmp, "Player%dのAction", Index);
	(*B_TextBox_pp)->AddStock(tmp);
	//////////////////////////////////////////
	
	if (NowTrick==NULL){
		Target = -1;
		ErrorDx("Error->CPlayer::Action->NowTrick=NULL(do nothing)");
		return true;
	}
	
	//とりあえずSINGLEの時だけ実装
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

	NowTrick = NULL;	//使用後は空に
	Target = -1;
	return true;
}


