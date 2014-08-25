#include "../Define.h"
#include "CmdList.h"
#include "CmdManager.h"
#include "Load.h"

#include "Field.h"
#include "Map.h"
#include "TextBox.h"
#include "EveManager.h"


//コマンドマネージャー：渡されたコマンドリストの命令を一行ずつすべて処理
void CCmdManager::Main(CCmdList* _cmdlist, index_tag _index, CField* _field, CMap* _map, CTextBox* _textbox, CEveManager* _evemanager){
	//渡されたコマンドリストの命令をすべて消費
		
		char commandline[256];	//コマンド行
		char command[256];	//@○○
		char *argument;	//引数
		
	while(!_cmdlist->Empty()){
		_cmdlist->Get(commandline);
		strcpy_s(command, commandline);

		//コマンド行をコマンド部と引数部に分離
			char *cntx;		//strtok_s用の雑用
			strtok_s(command, "(", &cntx);
			argument =  strchr(commandline, '(' );
			if (argument==NULL) {
				argument = command;
			}else{
				argument++;
				if (strrchr(argument, ')')==NULL) WarningDx("Warning->You may forget ')' in CommandText:%s", commandline);
				char *cntx;		//strtok_s用の雑用
				strtok_s(argument, ")", &cntx);
			}

		//場合によってはSolveの統合もあり	$
		switch(_index){
		case SYSTEM:
			Solve(command, argument, _field, _map, _evemanager);
			break;
		case ACTION:
			Solve(command, argument, _field, _map, _textbox, _evemanager);
			break;
		default:
			break;
		}
	
	}
}

//////////////////////////////////////////////////////////////////
//システムコマンドの処理//////////////////////////////////////////
//////////////////////////////////////////////////////////////////
void CCmdManager::Solve(const char* _command, char* _argument, CField* _field, CMap* _map, CEveManager* _evemanager){
	int argnum=0;	char** arg;
		
	if (strlen(_command)==0){
		ErrorDx("Error->strlen(_command)==0->%s", __FILE__, __LINE__, _command);
		return;

//@Load_Chip
	}else if (mystrcmp(_command,"@Load_Chip")){		
		argnum = 3;		arg = new char*[argnum];	ArgCut(_command, _argument, arg, argnum);	//必須
		
		long int num[2];	
		for (int i=0; (i<ARRAY_SIZE(num)&&i<argnum-1); i++){
			if(!( mystrtol(arg[i+1], &num[i]))){
				ErrorDx("Error->Could not change argument type->%s", __FILE__, __LINE__, _command);
				goto finish;
			}
		}
		_map->LoadChip(arg[0], (int)num[0], ((num[1]==0)?(true):(false))); 
		
//@Load_Map
	}else if (mystrcmp(_command, "@Load_Map")){
		argnum = 2;		arg = new char*[argnum];	ArgCut(_command, _argument, arg, argnum);	//必須

		long int mapnum;
		if(!( mystrtol(arg[1], &mapnum))){
			ErrorDx("Error->Check argument type->%s", __FILE__, __LINE__, _command);
			goto finish;
		}
		CLoad load;
		load.LoadMap(arg[0], (int)mapnum, _map, _evemanager, false);
		load.LoadMap(arg[0], (int)mapnum, _map, _evemanager, true);

//@Load_Pic
	}else if (mystrcmp(_command, "@Load_Pic")){
		argnum = 3;		arg = new char*[argnum];	ArgCut(_command, _argument, arg, argnum, false);	//必須

		_map->LoadPic(arg[0], arg[1], arg[2]);
		
//@Set_EventObj
	}else if (mystrcmp(_command,"@Set_EventObj")){
		argnum = 6;		arg = new char*[argnum];	ArgCut(_command, _argument, arg, argnum, false);	//必須
		
		long int num[2];
		for (int i=0; i<ARRAY_SIZE(num); i++){
			if(!( mystrtol(arg[i], &num[i]))){
				ErrorDx("Error->Could not change argument type->%s", __FILE__, __LINE__, _command);
				goto finish;
			}
		}
		int kind=0;
			if( mystrcmp(arg[2], "NPC") ){
				kind = NPC;
			}else if( mystrcmp(arg[2], "BLOCK") ){
				kind = BLOCK;
			}else if( mystrcmp(arg[2], "PANEL") ){
				kind = PANEL;
			}else if( mystrcmp(arg[2], "COVER") ){
				kind = COVER;
			}else if( mystrcmp(arg[2], "PUSHBLOCK") ){
				kind = PUSHBLOCK;

			}else{
				WarningDx("Warning->Not Match arg[Kind] (BLOCK):%s", __FILE__, __LINE__, arg[2]);
				mystrcpy(arg[2], "BLOCK");
				kind = BLOCK;
			}
			
		char numname[16];
		if (mystrcmp(arg[4], "NULL") || arg[4]==NULL) {		//Nameを指定していない時は"Kind-Mapnum-Datanum"をNameにする
			strcpy_s(numname, arg[2]);	strcat_s(numname, "-"); strcat_s(numname, arg[0]);	strcat_s(numname, "-"); strcat_s(numname, arg[1]);
		}

		bool visible = true;
		if (mystrcmp(arg[5], 'p', 3, "false", "FALSE", "False")) visible = false;

		_evemanager->SetEveObj((int)num[0], (int)num[1], kind, _map->GetImgData(arg[3]), arg[3], ((mystrcmp(arg[4], "NULL")||arg[4]==NULL)? numname : arg[4]), visible);

//@Set_StartPosition
	}else if (mystrcmp(_command,"@Set_StartPosition")){
		argnum = 5;		arg = new char*[argnum];	ArgCut(_command, _argument, arg, argnum);	//必須

		long int pos[3];
		for (int i=0; i<3; i++){
			if(!( mystrtol(arg[i], &pos[i]))){
				ErrorDx("Error->Check argument type->%s", __FILE__, __LINE__, _command);
				goto finish;
			}
		}
		_field->SetPosition((int)pos[0], (int)pos[1], (int)pos[2]);
		_field->SetMyDir(sys::StrtoDir(arg[3]));
		_field->SetMyPic(_map->GetImgData(arg[4]), arg[4]);
			

//コマンド不一致
	}else if (mystrcmp(_command,"")){
		WarningDx("Warning->Null command->%s", _command);	
		return;
	}else{
		WarningDx("Warning->Unregisterd command->%s", _command);
		return;
	}

finish:
	delete [] arg;

}

//////////////////////////////////////////////////////////////////
//シナリオコマンド（アクションコマンド）の処理////////////////////
//////////////////////////////////////////////////////////////////
void CCmdManager::Solve(const char* _command, char* _argument, CField* _field, CMap* _map, CTextBox* _textbox, CEveManager* _evemanager){
	int argnum=0;	char** arg;

	if (strlen(_command)==0){
		ErrorDx("Error->strlen(_command)==0->%s", __FILE__, __LINE__, _command);
		return;

//@EventWrap
	}else if (mystrcmp(_command, "@EventWrap",'l')){
		argnum = 1;		arg = new char*[argnum];//	ArgCut(_command, _argument, arg, argnum);	//必須のはずがまさかの例外
		char tmp[256];
		sprintf_s(tmp, "@EventWrap(%s)", _argument);	//完全な形でコマンドが必要なため
		_field->ChangeTextMode(false, tmp);

//@TextMode_Set
	}else if (mystrcmp(_command, "@TextMode_Set",'l')){
		argnum = 1;		arg = new char*[argnum];	ArgCut(_command, _argument, arg, argnum);	//必須

		if (mystrcmp(arg[0], 'p', 3, "BOX", "Box", "box")){
			_field->ChangeTextMode(true);
		}else if(mystrcmp(arg[0], 'p', 3, "WRAP", "Wrap", "wrap")){
			_field->ChangeTextMode(false);
		}else{
			ErrorDx("Warning->Check arg[mode]->%s", arg[0]);
			_field->ChangeTextMode(true);
		}
		
//@TName_Add
	}else if (mystrcmp(_command, "@TName_Add",'l')){
		argnum = 11;		arg = new char*[argnum];	ArgCut(_command, _argument, arg, argnum, false);	//必須

		bool left;
		if (sys::StrtoDir(arg[0])==LEFT){
			left=true;
		}else if (sys::StrtoDir(arg[0])==RIGHT){
			left=false;
		}else{
			WarningDx("Warning->@TName_Add->check arg[LEFTorRIGHT](right) :%s", arg[0]);
			left = false;
		}

		for (int i=1; i<argnum; i++){
			if (arg[i]!=NULL){	
				_textbox->TalkName.Add(left, 1, arg[i]);
			}
		}

//@TName_Dec
	}else if (mystrcmp(_command, "@TName_Dec",'l')){
		argnum = 11;		arg = new char*[argnum];	ArgCut(_command, _argument, arg, argnum, false);	//必須

		bool left;
		if (sys::StrtoDir(arg[0])==LEFT){
			left=true;
		}else if (sys::StrtoDir(arg[0])==RIGHT){
			left=false;
		}else{
			WarningDx("Warning->@TName_Dec->check arg[LEFTorRIGHT](right) :%s", arg[0]);
			left = false;
		}
		
		for (int i=1; i<argnum; i++){
			if (arg[i]!=NULL){	
				_textbox->TalkName.Dec(left, 1, arg[i]);
			}
		}
		
//@TName_Clear
	}else if (mystrcmp(_command, "@TName_Clear",'l')){
		argnum = 1;		arg = new char*[argnum];	ArgCut(_command, _argument, arg, argnum, false);	//必須
		
		if (mystrcmp(arg[0], 'p', 3, "ALL", "All", "all")){
			_textbox->TalkName.Clear(true);
			_textbox->TalkName.Clear(false);
		}else if (sys::StrtoDir(arg[0])==LEFT){
			_textbox->TalkName.Clear(true);
		}else if (sys::StrtoDir(arg[0])==RIGHT){
			_textbox->TalkName.Clear(false);
		}else{
			WarningDx("Warning->@TName_Clear->check arg[LEFTorRIGHTorALL](all) :%s", arg[0]);
			_textbox->TalkName.Clear(true);
			_textbox->TalkName.Clear(false);
		}
		
//@TName_Now
	}else if (mystrcmp(_command, "@TName_Now",'l')){
		argnum = 2;		arg = new char*[argnum];	ArgCut(_command, _argument, arg, argnum, false);	//必須

		bool left;
		if (sys::StrtoDir(arg[0])==LEFT){
			left=true;
		}else if (sys::StrtoDir(arg[0])==RIGHT){
			left=false;
		}else{
			WarningDx("Warning->@TName_Now->check arg[LEFTorRIGHT](right) :%s", arg[0]);
			left = false;
		}

		if (arg[1]!=NULL){
			_textbox->TalkName.SetNowName(left, arg[1], true);
		}else{
			_textbox->TalkName.SetNowSide(left);
		}	

//@AutoPlay_Set
	}else if (mystrcmp(_command, "@AutoPlay_Set",'l')){
		argnum = 2;		arg = new char*[argnum];	ArgCut(_command, _argument, arg, argnum, false);	//必須

		int num;
		if (!mystrtol(arg[1], &num)) num = NULL;

		if (sys::TrueOrFalse(arg[0], true)){
			_textbox->SetAutoPlay(true, num);
		}else if (sys::TrueOrFalse(arg[0], false)){
			_textbox->SetAutoPlay(false);
		}else{
			WarningDx("WarningDx->@AutoPlay_Set（false）: Check arg[TrueOrFalse]:%s", arg[0]);
			_textbox->SetAutoPlay(false);
		}


//@Dir_Set
	}else if (mystrcmp(_command, "@Dir_Set",'l')){
		argnum = 2;		arg = new char*[argnum];	ArgCut(_command, _argument, arg, argnum);	//必須

		if( sys::PlayerName(arg[0]) ){
			_field->SetMyDir(sys::StrtoDir(arg[1] ,_textbox->GetOriginalDir()));
		}else{
			_evemanager->SetDir(arg[0], sys::StrtoDir(arg[1] ,_textbox->GetOriginalDir()));
		}

//@Visible_Set
	}else if (mystrcmp(_command, "@Visible_Set",'l')){
		argnum = 2;		arg = new char*[argnum];	ArgCut(_command, _argument, arg, argnum);	//必須

		bool npc;
		if( sys::PlayerName(arg[0]) ){	npc = false; }else{	npc = true;	}

		bool visible;
		if (sys::TrueOrFalse(arg[1], true)){				visible=true;
		}else if (sys::TrueOrFalse(arg[1], false)){		visible=false;
		}else{
			ErrorDx("ErrorDx->@Visible_Set（false）: Check arg[visible]:%s", arg[1]);
			visible=false;
		}
		
		if (mystrcmp(arg[0], 'p', 3, "TEXTBOX", "textbox", "TextBox")) {
			_textbox->SetVisible(visible);
		}else if (npc){
			_evemanager->SetVisible(arg[0], visible); 
		}else{
			_field->SetMyVisible(visible);
		}

//@Alpha_Set
	}else if (mystrcmp(_command, "@Alpha_Set",'l')){
		argnum = 2;		arg = new char*[argnum];	ArgCut(_command, _argument, arg, argnum);	//必須

		int per;
		if (!(mystrtol(arg[1], &per))){
			ErrorDx("Error->@Alpha_Set->Check type arg[per]->%s", __FILE__, __LINE__, arg[1]);
		}else{
			if (per<0 || per>100) {
				ErrorDx("Error->@Alpha_Set-> 0<=|arg[per]|<=100 :%d", per);
			}else{
				if( sys::PlayerName(arg[0]) ){
					ErrorDx("Error->@AlphaSet->You can't change Player Alpha:%s", arg[0]);
				}else{
					_evemanager->SetAlpha(arg[0], per*255/100);
				}
			}
		}

//@Effect_Set
	}else if (mystrcmp(_command, "@Effect_Set",'l')){
		argnum = 7;		arg = new char*[argnum];	ArgCut(_command, _argument, arg, argnum, false);	//必須

		int num[5] = {-1,-1,-1,-1,-1};
		charaeffect_tag effect;
		int arg_num;

		int effectnum;
		if (!(mystrtol(arg[1], &effectnum))) effectnum=EFFECT_NUM;
		
		if (mystrcmp(arg[1], 'p', 3, "TMP", "Tmp", "tmp") || effectnum==-1){
			_evemanager->SetEffect(arg[0], -1, num);		//TextBox.Termからの呼び出し
			goto finish;
		}else if (mystrcmp(arg[1], 'p', 3, "NONE", "None", "none") || effectnum==NONE){
			effect = NONE;
			arg_num=0;
		}else if (mystrcmp(arg[1], 'p', 3, "BLINK", "Blink", "blink") || effectnum==BLINK){
			effect = BLINK;
			arg_num=3;
		}else if (mystrcmp(arg[1], 'p', 4, "RND_DIR", "Rnd_dir", "Rnd_Dir", "rnd_dir") || effectnum==RND_DIR){
			effect = RND_DIR;
			arg_num=1;
		}else if (mystrcmp(arg[1], 'p', 3, "WALK", "Walk", "walk") || effectnum==WALK){
			effect = WALK;
			arg_num = 3;
		}else{
			ErrorDx("Error->@Effect_Set->Check EffectName:%s", arg[0], effectnum, arg[1]);
			goto finish;
		}
		
		for (int i=0; i<arg_num; i++){
			if(!( mystrtol(arg[i+2], &num[i]))){
				ErrorDx("Error->@Effect_Set->Check type:%s", __FILE__, __LINE__, arg[i+2]);
				goto finish;
			}
		}
		_evemanager->SetEffect(arg[0], effect, num);

//@Pic_Set
	}else if (mystrcmp(_command, "@Pic_Set",'l')){
		argnum = 2;		arg = new char*[argnum];	ArgCut(_command, _argument, arg, argnum);	//必須
		
		if( sys::PlayerName(arg[0]) ){
			_field->SetMyPic(_map->GetImgData(arg[1]), arg[1]);
		}else{		
			_evemanager->SetPic(arg[0], _map->GetImgData(arg[1]), arg[1]);
		}

//@Flag_Set
	}else if (mystrcmp(_command, "@Flag_Set",'l')){
		argnum = 2;		arg = new char*[argnum];	ArgCut(_command, _argument, arg, argnum);	//必須

		int num=0;	char* p;
		if(mystrtol(arg[1], &num)){
			_field->FlagSet.SetFlag(arg[0], num);
		
		}else if ((p=strchr(arg[1],'+'))!=NULL){
			while(p!=NULL){
				num++;
				p=strchr(++p,'+');
			}
			_field->FlagSet.SetFlag(arg[0], num, true);

		}else if ((p=strchr(arg[1],'-'))!=NULL){
			while(p!=NULL){
				num--;
				p=strchr(++p,'-');
			}
			_field->FlagSet.SetFlag(arg[0], num, true);

		}else{
			ErrorDx("Error->@Flag_Set->Check arg[num]->%s", __FILE__, __LINE__, arg[1]);
			goto finish;
		}

//@Count_Set
	}else if (mystrcmp(_command, "@Count_Set",'l')){
		argnum = 2;		arg = new char*[argnum];	ArgCut(_command, _argument, arg, argnum);	//必須

		int num=0;	char* p;
		if(mystrtol(arg[1], &num)){
			_evemanager->SetCount(arg[0], num);
		
		}else if ((p=strchr(arg[1],'+'))!=NULL){
			while(p!=NULL){
				num++;
				p=strchr(++p,'+');
			}
			_evemanager->SetCount(arg[0], num, true);

		}else if ((p=strchr(arg[1],'-'))!=NULL){
			while(p!=NULL){
				num--;
				p=strchr(++p,'-');
			}
			_evemanager->SetCount(arg[0], num, true);

		}else{
			ErrorDx("Error->@Count_Set->Check arg[num]->%s", __FILE__, __LINE__, arg[1]);
			goto finish;
		}
		
//@Position_Set
	}else if (mystrcmp(_command,"@Position_Set")){
		argnum = 5;		arg = new char*[argnum];	ArgCut(_command, _argument, arg, argnum, false);	//必須

		if( sys::PlayerName(arg[0]) ){
			if (arg[3]!=NULL){		//@Position_Set(playername, mapnum, dx, dy)

				int pos[3];
				if (!(mystrtol(arg[1], &pos[0]))) pos[0] = -1;
				for (int i=1; i<3; i++){
					if(!( mystrtol(arg[i+1], &pos[i]))){
						ErrorDx("Error->@Position_Set->Check type:%s", __FILE__, __LINE__, arg[i+1]);
						goto finish;
					}
				}
				if (strchr(arg[2], '+')!=NULL || strchr(arg[2], '-')!=NULL ||
					strchr(arg[3], '+')!=NULL || strchr(arg[3], '-')!=NULL) {
					_field->SetPosition((int)pos[0], (int)pos[1], (int)pos[2], true);
				}else{
					_field->SetPosition((int)pos[0], (int)pos[1], (int)pos[2]);
				}
				if (arg[4]!=NULL) _field->SetMyPic(_map->GetImgData(arg[4]), arg[4]);

			}else if(arg[2]!=NULL){				//@Position_Set(playername, mapnum, datanum)

				int num[2];
				if (!(mystrtol(arg[1], &num[0]))) num[0] = -1;
				if(!( mystrtol(arg[2], &num[1]))){
					ErrorDx("Error->@Position_Set->Check type: %d", __FILE__, __LINE__, arg[2]);
					goto finish;
				}
				int x, y;
				if (_evemanager->GetPos(num[0], num[1], &x, &y)){
					_field->SetPosition(num[0], x, y);
				}

			}else{								//@Position_Set(playername, eveobjname)
				
				int mapnum, x, y;
				if (_evemanager->GetPos(arg[1], &mapnum, &x, &y)){
					_field->SetPosition(mapnum, x, y);
				}		
			}

		}else{
			if (arg[2]!=NULL){	//@Position_Set(eventobjname, dx,dy, trueorfalse)
			
				int pos[2];
				for (int i=0; i<2; i++){
					if(!( mystrtol(arg[i+1], &pos[i]))){
						ErrorDx("Error->@Position_Set->Check type:%s", __FILE__, __LINE__, arg[i+1]);
						goto finish;
					}
				}
				if (arg[3]!=NULL && sys::TrueOrFalse(arg[3], true)){
					_evemanager->SetPosition(arg[0], (int)pos[0], (int)pos[1], true);
				}else{
					_evemanager->SetPosition(arg[0], (int)pos[0], (int)pos[1], false);
				}

			}else{	//@Position_Set(eventobjname, datanum/eventobjname)
				
				int num;
				if( mystrtol(arg[1], &num)){
					_evemanager->SetPosition(arg[0], num);
				}else{
					_evemanager->SetPosition(arg[0], arg[1]);
				}
			}
		}
	
//140420 EveManagerのTmpEffectとの兼ね合いのため削除
////@JokerName_Set
//	}else if (mystrcmp(_command, "@JokerName_Set",'l')){
//		argnum = 1;		arg = new char*[argnum];	ArgCut(_command, _argument, arg, argnum);	//必須
//
//		_evemanager->SetNowName(arg[0]);
		
//@Wait
	}else if (mystrcmp(_command, "@Wait",'l')){
		argnum = 1;		arg = new char*[argnum];	ArgCut(_command, _argument, arg, argnum);	//必須

		long int num;
		if(!( mystrtol(arg[0], &num))){
			ErrorDx("Error->Check argument type->%s", __FILE__, __LINE__, _command);
			goto finish;
		}else{
			if (num<0 || num>10000) WarningDx("Warning->@Wait 0<=|arg[time miliseconds]|<=10000: %d", __FILE__, __LINE__, (int)num); 
			num = between(0, 10000, (int)num);
		}

		_field->Draw(true, true);
		WaitTimer(num);		//一定時間経過するまでwhileループさせてDrawする方が各種アニメーションが止まらないためいいのでは？←必要ならどっちも作っちゃえよ$

//@Anten
	}else if (mystrcmp(_command, "@Anten",'l')){
		argnum = 1;		arg = new char*[argnum];	ArgCut(_command, _argument, arg, argnum);	//必須

		long int num;
		if(!( mystrtol(arg[0], &num))){
			ErrorDx("Error->Check argument type->%s", __FILE__, __LINE__, _command);
			goto finish;
		}else{
			if (num<1 || num>10000) WarningDx("Warning->@Anten 0<|arg[time miliseconds]|<=10000: %d", __FILE__, __LINE__, (int)num); 
			num = between(1, 10000, (int)num);
		}

		_field->FadeDraw(num, BLACK, true, true);
	
//@Meiten
	}else if (mystrcmp(_command, "@Meiten",'l')){
		argnum = 1;		arg = new char*[argnum];	ArgCut(_command, _argument, arg, argnum);	//必須

		long int num;
		if(!( mystrtol(arg[0], &num))){
			ErrorDx("Error->Check argument type->%s", __FILE__, __LINE__, _command);
			goto finish;
		}else{
			if (num<1 || num>10000) WarningDx("Warning->@Meiten 0<|arg[time miliseconds]|<=10000: %d", __FILE__, __LINE__, (int)num); 
			num = between(1, 10000, (int)num);
		}

		_field->FadeDraw(num, BLACK, false, true);
		
//@BackGround_Change
	}else if (mystrcmp(_command, "@BackGround_Change",'l')){
		argnum = 2;		arg = new char*[argnum];	ArgCut(_command, _argument, arg, argnum);	//必須

		int num;
		if(!( mystrtol(arg[1], &num))){
			ErrorDx("Error->Check argument type->%s", __FILE__, __LINE__, _command);
			goto finish;
		}else{
			if (num>10000) WarningDx("Warning->@BackGround_Change arg[time miliseconds]<=10000: %d", __FILE__, __LINE__, num); 
			num = min(10000, num);
		}
		
		int img;
		if ((img = _map->GetImgData(arg[0])[0]) == NULL){
			_field->FadeDraw(num, img, false, false);	
		}else{
			_field->FadeDraw(num, img, true, false);	
		}

//@Jump
	}else if (mystrcmp(_command, "@Jump",'l')){
		argnum = 1;		arg = new char*[argnum];	ArgCut(_command, _argument, arg, argnum);	//必須

		if( sys::PlayerName(arg[0])){
			_field->Jump();
		}else{
			_evemanager->Jump(_field, arg[0]);
		}

//@Walk
	}else if (mystrcmp(_command, "@Walk",'l')){
		argnum = 4;		arg = new char*[argnum];	ArgCut(_command, _argument, arg, argnum, false);	//必須

		int walkspeed;
		walkspeed = choose(sys::rank3(arg[3]), 1, 2, 4);
		
		long int cell;
		if(!( mystrtol(arg[2], &cell))){
			ErrorDx("Error->Check argument type->%s", __FILE__, __LINE__, _command);
			goto finish;
		}
		if (cell<0) {
			walkspeed=-walkspeed;
			cell=-cell;
		}
		if (cell>100) WarningDx("Warning->@Walk arg[cell]<=100: %d", __FILE__, __LINE__, cell); 
		cell = between(1, 100, (int)cell);
		
		for (int i=0; i<cell; i++){
			if( sys::PlayerName(arg[0]) ){
				_field->Walk(sys::StrtoDir(arg[1],_textbox->GetOriginalDir()), walkspeed, true, true);
			}else{
				_evemanager->Walk(_field, arg[0], sys::StrtoDir(arg[1],_textbox->GetOriginalDir()), walkspeed, true);
			}
		}

//@Slide
	}else if (mystrcmp(_command, "@Slide",'p')){
		argnum = 4;		arg = new char*[argnum];	ArgCut(_command, _argument, arg, argnum, false);	//必須
		
		int walkspeed;
		walkspeed = choose(sys::rank3(arg[3]), 2, 4, 8);

		int dir;
		dir = sys::StrtoDir(arg[1],_textbox->GetOriginalDir());

		int cell;
		if(!( mystrtol(arg[2], &cell))){
			ErrorDx("Error->Check argument type[cell]->%s", __FILE__, __LINE__, _command);
			goto finish;
		}else{
			if (cell<0) {
				dir = sys::TurnDir(dir, 2);
				cell = -cell;
			}
			if (cell>100) WarningDx("Warning->@Slide |arg[cell]|<=100: %d", __FILE__, __LINE__, cell); 
			cell = between(1, 100, cell);
		}

		if( sys::PlayerName(arg[0]) ){
			for (int i=0; i<cell; i++){
				_field->Walk(dir, walkspeed, true, false);
			}
		
		}else{
			int mapnum=-1, x=-1, y=-1;
			if (!_evemanager->GetPos(arg[0], &mapnum, &x, &y)){
				goto finish;
			}
			
			for (int i=0; i<cell; i++){
				x+=((dir==RIGHT)? 1: ((dir==LEFT)? -1: 0));
				y+=((dir==DOWN)? 1: ((dir==UP)? -1: 0));
				if (x<0 || x>=MAP_SIZE || y<0 || y>=MAP_SIZE || mapnum<0 || mapnum>=MAP_MAX){
					break;
				}
				if (_map->GetMapData(mapnum, x%MAP_SIZE, y%MAP_SIZE, 1)	//障害物の有無を確認
					|| (!_evemanager->CheckWalkable(mapnum, x%MAP_SIZE, y%MAP_SIZE))){	//NPCorBLOCKorPUSHBLOCKor...の有無を確認
						break;
				}
				_evemanager->Walk(_field, arg[0], dir, walkspeed, false);
			}
		}

//@FadeWalk(name, dir, speed, fade)
	}else if (mystrcmp(_command, "@FadeWalk",'p')){
		argnum = 4;		arg = new char*[argnum];	ArgCut(_command, _argument, arg, argnum, false);	//必須
		
		int walkspeed;
		walkspeed = choose(sys::rank3(arg[3]), 1, 2, 4);

		int fade;
		if (!mystrtol(arg[2], &fade) || (fade!=-1 && fade!=1)) {
			fade=0;
			WarningDx("Warning->@FadeWalk->arg[fade] should 1 or -1:  %s", arg[2]); 
		}

		if( sys::PlayerName(arg[0]) ){
			_field->Walk(sys::StrtoDir(arg[1],_textbox->GetOriginalDir()), walkspeed, true, true, fade);
		}else{
			_evemanager->Walk(_field, arg[0], sys::StrtoDir(arg[1],_textbox->GetOriginalDir()), walkspeed, true, fade);
		}

//@Window_Shake
	}else if (mystrcmp(_command, "@Window_Shake",'p')){
		argnum = 2;		arg = new char*[argnum];	ArgCut(_command, _argument, arg, argnum);	//必須
		
		int num[2];
		for (int i=0; i<2; i++){
			if(!( mystrtol(arg[i], &num[i]))){
				ErrorDx("Error->Check argument type->%s", __FILE__, __LINE__, _command);
				goto finish;
			}
		}

		if (num[0]<0 || num[0]>1000)   WarningDx("Warning->@Window_Shake 0<=|arg[cell]|<=100: %d", __FILE__, __LINE__, num[0]); 
		if (num[1]<-100 || num[1]>100) WarningDx("Warning->@Window_Shake -100<=|arg[cell]|<=100: %d", __FILE__, __LINE__, num[0]); 
		
		num[0] = between(0, 1000, num[0]);
		num[1] = between(-100, 100, num[1]);

		int d=0;
		for (int i=0; i<num[0]; i++){
			d = num[1]*(i%3-1);
			_field->Draw(true, true, 0, d, true);
		}

//@Window_MoveTurn
	}else if (mystrcmp(_command, "@Window_MoveTurn",'l')){
		argnum = 4;		arg = new char*[argnum];	ArgCut(_command, _argument, arg, argnum, false);	//必須

		int speed;
		speed = choose(sys::rank3(arg[3]), 2, 4, 8);

		direction_tag dir = sys::StrtoDir(arg[0],_textbox->GetOriginalDir());
		
		int cell;
		if(!( mystrtol(arg[1], &cell))){
			ErrorDx("Error->Check argument type[cell]->%s", __FILE__, __LINE__, _command);
			goto finish;
		}else{		
			if (cell<0){
				cell = -cell;
				dir = sys::TurnDir(dir, 2);
			}
			if (cell>100) WarningDx("Warning->@Window_MoveTurn |arg[cell]|<=100: %d", __FILE__, __LINE__, cell); 
			cell = between(0, 100, cell);
		}

		int waittime;
		if(!( mystrtol(arg[2], &waittime))){
			ErrorDx("Error->Check argument type[waittime]->%s", __FILE__, __LINE__, _command);
			goto finish;
		}else{
			if (waittime<0 || waittime>10000) WarningDx("Warning->@Window_MoveTurn 0<=|arg[waittime]|<=10000: %d", __FILE__, __LINE__, waittime); 
			waittime = between(0, 10000, waittime);
		}

		int d=0;
		while (d<cell*MAP_CHIP_SIZE){
			d+=speed;
			_field->Draw(true, true, (dir==RIGHT? d: (dir==LEFT? -d: 0)), (dir==DOWN? d: (dir==UP? -d: 0)), true);
		}
		WaitTimer(waittime);
		while (d>0){
			d-=speed;
			_field->Draw(true, true, (dir==RIGHT? d: (dir==LEFT? -d: 0)), (dir==DOWN? d: (dir==UP? -d: 0)), true);
		}

//@Window_Move
	}else if (mystrcmp(_command, "@Window_Move",'l')){
		argnum = 4;		arg = new char*[argnum];	ArgCut(_command, _argument, arg, argnum, false);	//必須

		int speed;
		speed = choose(sys::rank3(arg[2]), 2, 4, 8);
		
		int cell;
		if(!( mystrtol(arg[1], &cell))){
			ErrorDx("Error->Check argument type[cell]->%s", __FILE__, __LINE__, _command);
			goto finish;
		}else{
			if (cell<0 || cell>100) WarningDx("Warning->@Window_Move 0<=|arg[cell]|<=100: %d", __FILE__, __LINE__, cell); 
			cell = between(1, 100, cell);
		}
		direction_tag dir = sys::StrtoDir(arg[0],_textbox->GetOriginalDir());
		if (arg[3]==NULL){	//通常移動
			int d=0;
			while (d<cell*MAP_CHIP_SIZE){
				d+=speed;
				_field->Draw(true, true, (dir==RIGHT? d: (dir==LEFT? -d: 0)), (dir==DOWN? d: (dir==UP? -d: 0)), true);
			}

		}else{	//back
			if (!(mystrcmp(arg[3], 'p', 3, "BACK", "Back", "back"))) WarningDx("Warning->@Window_Move->Check arg[option_back]: ", arg[3]);
			int d=cell*MAP_CHIP_SIZE;
			while (d>0){
				d-=speed;
				_field->Draw(true, true, (dir==RIGHT? -d: (dir==LEFT? d: 0)), (dir==DOWN? -d: (dir==UP? d: 0)), true);
			}
		}

//コマンド不一致
	}else if (mystrcmp(_command,"")){
		WarningDx("Warning->Null command->%s", _command);	
		return;
	}else{
		WarningDx("Warning->Unregisterd command->%s", _command);
		return;
	}
	
finish:
	delete [] arg;
}


//引数分解：区切り文字CMD_SEPARATORはDefine.hで定義
void CCmdManager::ArgCut(const char* _command, char* _argument, char** _arg, int _argnum, bool _warning){

	char *cntx;		//strtok_s用の雑用
	_arg[0] = strtok_s(_argument, CMD_SEPARATOR, &cntx );
		
	for (int i=1; i<_argnum; i++){
		_arg[i] = strtok_s( NULL, CMD_SEPARATOR , &cntx);
		if( _arg[i] == NULL && _warning){
			ErrorDx("Error->More arguments are needed->%s", _command);
			return;
		}
	}
	
	char* exarg;
	if (NULL!=(exarg=strtok_s( NULL, CMD_SEPARATOR , &cntx))){
		WarningDx("Warning->Too many arguments exist:%s", _command);
	}

	return ;
}

////////////////////////////////////////////////////////////////////////////////////////
///コマンドリスト///////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////


//最後尾に命令を追加
void CCmdList::Add(const char* _data){

	if( strlen(_data)>256 ){
		ErrorDx("Error->CmdList::Add 字数オーバー[255]：%s", __FILE__, __LINE__, _data);
		return;
	}

	node_tag* p = new node_tag;
	mystrcpy(p->command, _data);
	p->next = NULL;

	if (tail!=NULL)	tail->next = p;
	tail = p;

	if (front==NULL) front = p;
}


//先頭行の命令を返す
void CCmdList::Get(char* _cmd){
	node_tag* p = front;

	mystrcpy(_cmd, p->command);
	front = p->next;	
	if (p==tail) tail = NULL;

	delete p;
}
bool CCmdList::Empty(){
	return (front == NULL);
}


void CCmdList::Clear(){
	
	node_tag* p=front;
	node_tag* tmp;

	/* 連結リストの要素を削除 */
	while( p != NULL ){
		tmp = p->next;
		delete p;
		p = tmp; 
	}
	
	front = NULL;
	tail = NULL;
}