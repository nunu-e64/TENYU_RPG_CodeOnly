#include "../Define.h"
#include "CmdList.h"
#include "CmdManager.h"
#include "TextBox.h"

#include "../Field/Field.h"
#include "../Field/Load.h"
#include "../Field/Map.h"
#include "../Field/EveManager.h"

#include "../Battle/Battle.h"

void CFirstSetCmdManager::Main(CCmdList* _cmdlist, CField* _field, CMap* _map, CEveManager* _evemanager){

	char commandline[256];	//コマンド行
	char command[256];		//@○○
	char *argument=NULL;	//引数	//コンパイル警告が鬱陶しいので初期化しておいてあげる

	while (NextCommand(_cmdlist, commandline, command, argument)){
		if (!SystemCmdSolve(command, argument, _field, _map, _evemanager)){
			WarningDx("Warning->CFirstSetCmdManager->Unregisterd command->%s", command);
		}
	}
}

void CFieldCmdManager::Main(CCmdList* _cmdlist, CField* _field, CMap* _map, CTextBox* _textbox, CEveManager* _evemanager){
	
	char commandline[256];	//コマンド行
	char command[256];	//@○○
	char *argument;	//引数

	while (NextCommand(_cmdlist, commandline, command, argument) && _field->GetGameMode()==MODE_PLAYING){	//Modeが変更されてGameOverとかしていたら強制終了
		if (!FieldCmdSolve (command, argument, _field, _map, _textbox, _evemanager) 
		 && !WindowCmdSolve(command, argument, _field, _map, _textbox)
		 && !TextCmdSolve  (command, argument, _field, _textbox)){
			WarningDx("Warning->CFieldCmdManager->Unregisterd command->%s", command);
		}
	}
}

void CBattleCmdManager::Main(CCmdList* _cmdlist, CBattle* _battle, CTextBox* _textbox){
	
	char commandline[256];	//コマンド行
	char command[256];	//@○○
	char *argument;	//引数

	while (NextCommand(_cmdlist, commandline, command, argument)){
		if (!WindowCmdSolve(command, argument, _battle, Map_p, _textbox)
		 && !TextCmdSolve  (command, argument, _battle, _textbox)
		 && !BattleCmdSolve(command, argument, _battle)){
			WarningDx("Warning->CBattleCmdManager->Unregisterd command->%s", command);
		}
	}
}

void CBattleFirstSetCmdManager::Main(CCmdList* _cmdlist, CBImgBank* _bimgbank, CPlayerSpeciesManager* _playerSpeciesManager, CEnemySpeciesManager* _enemySpeciesManager, CTrickManager* _trickManager){
	
	char commandline[256];	//コマンド行
	char command[256];	//@○○
	char *argument;	//引数

	while (NextCommand(_cmdlist, commandline, command, argument)){
		if (!BattleSystemCmdSolve(command, argument, _bimgbank, _playerSpeciesManager, _enemySpeciesManager, _trickManager)){
			WarningDx("Warning->CBattleFirstSetCmdManager->Unregisterd command->%s", command);
		}
	}
}

bool CCmdManager::NextCommand(CCmdList* _cmdlist, char* commandline, char* command, char* &argument){ 
		
	while(!_cmdlist->Empty()){
		_cmdlist->Get(commandline);
		mystrcpy(command, commandline);

		//コマンド行をコマンド部と引数部に分離
			char *cntx;		//strtok_s用の雑用
			strtok_s(command, "(", &cntx);
			argument =  strchr(commandline, '(' );
			if (argument==NULL) {
				argument = command;
			}else{
				argument++;
				char* p;
				if ((p = strrchr(argument, ')'))!=NULL) {
					*p = '\0';					
					//char *cntx;		//strtok_s用の雑用
					//strtok_s(argument, ")", &cntx);
				}else{
					WarningDx("Warning->You may forget ')' in CommandText:%s", commandline);
				}
			}

		return true;
		
	}
	return false;
}


//引数分解：区切り文字CMD_SEPARATORはDefine.hで定義
bool CCmdManager::ArgCut(const char* _command, char* _argument, char** _arg, int _argnum, bool _warning){

	char *cntx;		//strtok_s用の雑用
	_arg[0] = strtok_s(_argument, CMD_SEPARATOR, &cntx );
		
	for (int i=1; i<_argnum; i++){
		_arg[i] = strtok_s( NULL, CMD_SEPARATOR , &cntx);
		if( _arg[i] == NULL && _warning){
			ErrorDx("Error->More arguments are needed->%s", _command);
			return false;
		}
	}
	
	char* exarg;
	if (NULL!=(exarg=strtok_s( NULL, CMD_SEPARATOR , &cntx))){
		WarningDx("Warning->Too many arguments exist:%s", _command);
	}

	return true;
}

//////////////////////////////////////////////////////////////////
//システムコマンドの処理//////////////////////////////////////////
//////////////////////////////////////////////////////////////////
bool CCmdManager::SystemCmdSolve(const char* _command, char* _argument, CField* _field, CMap* _map, CEveManager* _evemanager){
	int argnum=0;	char** arg;
		
	if (strlen(_command)==0){
		ErrorDx("Error->strlen(_command)==0->%s", __FILE__, __LINE__, _command);
		return true;

//@Load_Chip
	}else if (mystrcmp(_command,"@Load_Chip")){		
		argnum = 3;		arg = new char*[argnum];	if(!ArgCut(_command, _argument, arg, argnum))goto finish;	//必須
		
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
		argnum = 2;		arg = new char*[argnum];	if(!ArgCut(_command, _argument, arg, argnum))goto finish;	//必須

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
		argnum = 5;		arg = new char*[argnum];	if(!ArgCut(_command, _argument, arg, argnum))goto finish;	//必須

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
	}else{
		return false;
	}

finish:
	delete [] arg;
	return true;
}
bool CCmdManager::BattleSystemCmdSolve(const char* _command, char* _argument, CBImgBank* _bimgbank, CPlayerSpeciesManager* _playerSpeciesManager, CEnemySpeciesManager* _enemySpeciesManager, CTrickManager* _trickManager){
	int argnum=0;	char** arg;
		
	if (strlen(_command)==0){
		ErrorDx("Error->strlen(_command)==0->%s", __FILE__, __LINE__, _command);
		return true;
		
//@Load_Pic
	}else if (mystrcmp(_command, "@Load_Pic")){
		argnum = 2;		arg = new char*[argnum];	if(!ArgCut(_command, _argument, arg, argnum))goto finish;	//必須

		_bimgbank->Add(arg[1], LoadGraph(arg[0], true));
		//_map->LoadPic(arg[0], arg[1], arg[2]);

//@Player_Create
	}else if (mystrcmp(_command,"@Player_Create")){	
		argnum = 6;		arg = new char*[argnum];	if(!ArgCut(_command, _argument, arg, argnum))goto finish;	//必須

		int value[4];
		for (int i=0; i<4; i++){
			if(!( mystrtol(arg[i+1], &value[i]))){
				ErrorDx("Error->Check argument type->%s", __FILE__, __LINE__, _command);
				goto finish;
			}
		}
		_playerSpeciesManager->CreateSpecies(arg[0], value[0], value[1], value[2], value[3], _bimgbank->GetImg(arg[5]));

//@Enemy_Create
	}else if (mystrcmp(_command,"@Enemy_Create")){		
		argnum = 6;		arg = new char*[argnum];	if(!ArgCut(_command, _argument, arg, argnum))goto finish;	//必須

		int value[4];
		for (int i=0; i<4; i++){
			if(!( mystrtol(arg[i+1], &value[i]))){
				ErrorDx("Error->Check argument type->%s", __FILE__, __LINE__, _command);
				goto finish;
			}
		}
		_enemySpeciesManager->CreateSpecies(arg[0], value[0], value[1], value[2], value[3], _bimgbank->GetImg(arg[5]));

//@TrickEffect_Create
	}else if (mystrcmp(_command,"@TrickEffect_Create")){
		argnum = 2+10;		arg = new char*[argnum];	ArgCut(_command, _argument, arg, argnum, false);	//必須

		if (arg[0]==NULL){
			ErrorDx("Error->@TrickEffect_Create->arg[EffectName]=NULL", __FILE__, __LINE__);
			goto finish;
		}else if (arg[1]==NULL){
			ErrorDx("Error->@TrickEffect_Create('%s')->arg[EFFECT_TYPE]=NULL", __FILE__, __LINE__, arg[0]);
			goto finish;
		}

		std::vector <std::string> stringList;
		for (int i=2; i<argnum && arg[i]!=NULL; i++){
			stringList.push_back(arg[i]);
		}

		_trickManager->CreateDamageEffect(arg[1], arg[0], stringList);

//@NormalTrick_Create
	}else if (mystrcmp(_command,"@NormalTrick_Create")){
		argnum = 4;		arg = new char*[argnum];	ArgCut(_command, _argument, arg, argnum);	//必須

		int value[2];
		for (int i=0; i<2; i++){
			if(!( mystrtol(arg[i+1], &value[i]))){
				ErrorDx("Error->@NormalTrick_Create->Check argument type->%s", __FILE__, __LINE__, _command);
				goto finish;
			}
		}
		_trickManager->Add(arg[0], value[0], value[1], trick_tag::SINGLE, arg[3], 0);	

//@PlayerTrick_Set
	}else if (mystrcmp(_command,"@PlayerTrick_Set")){		
		argnum = 20;		arg = new char*[argnum];	ArgCut(_command, _argument, arg, argnum, false);	//必須

		if (arg[0]==NULL){
			ErrorDx("Error->@PlayerTrick_Set->arg[name]=NULL", __FILE__, __LINE__);

		}else if (arg[1]==NULL){
			ErrorDx("Error->@PlayerTrick_Set->arg[TrickName]=NULL", __FILE__, __LINE__);

		}else{

			if (arg[argnum-1]!=NULL) {
				WarningDx("Warning->@PlayerTrick_Set(%s)->Number of arg[trick] > max:%d(not register %s)", __FILE__, __LINE__, arg[0], argnum-3, arg[argnum-1]);
			}

			std::vector <trick_tag const*>trickList;
			trick_tag const* tmpTrick;
			for (int i=1; i<argnum && arg[i]!=NULL; i++){
				if ((tmpTrick = _trickManager->GetTrick(arg[i])) != NULL){
					trickList.push_back(tmpTrick);
				}				
			}
			_playerSpeciesManager->SetTrickList(arg[0], trickList);
		}

//@EnemyTrick_Set
	}else if (mystrcmp(_command,"@EnemyTrick_Set")){		
		argnum = 10+2+1;		arg = new char*[argnum];	ArgCut(_command, _argument, arg, argnum, false);	//必須

		if (arg[0]==NULL){
			ErrorDx("Error->@EnemyTrick_Set->arg[name]=NULL", __FILE__, __LINE__);
		
		}else if (arg[1]==NULL){
			ErrorDx("Error->@EnemyTrick_Set->arg[TrickName]=NULL", __FILE__, __LINE__);

		}else{
			
			if (arg[argnum-1]!=NULL) {
				WarningDx("Warning->@EnemyTrick_Set(%s)->Number of arg[trick] > max:%d(not register ""%s"")", __FILE__, __LINE__, arg[0], argnum-3, arg[argnum-1]);
			}

			std::vector <trick_tag const*>trickList;
			trick_tag const* tmpTrick;
			for (int i=1; i<argnum && arg[i]!=NULL; i++){
				if ((tmpTrick = _trickManager->GetTrick(arg[i])) != NULL){
					trickList.push_back(tmpTrick);
				}				
			}
			_enemySpeciesManager->SetTrickList(arg[0], trickList);
		}

//@RandomPlan_Set
	}else if (mystrcmp(_command,"@RandomPlan_Set")){		
		argnum = 20+2+1;		arg = new char*[argnum];	ArgCut(_command, _argument, arg, argnum, false);	//必須

		if (arg[0]==NULL){
			ErrorDx("Error->@RandomPlan_Set->arg[index]=NULL", __FILE__, __LINE__);
			goto finish;
		}else if (arg[1]==NULL){
			ErrorDx("Error->@RandomPlan_Set->arg[EnemyName]=NULL:arg[0]=%s", __FILE__, __LINE__, arg[0]);
			goto finish;
		}
		
		int index;
		if(!( mystrtol(arg[0], &index))){
			ErrorDx("Error->@RandomPlan_Set(%s,%s)->Check argument type[0]->%s", __FILE__, __LINE__, arg[0], arg[1], _argument);
			goto finish;
		}else if (index<0){
			ErrorDx("Error->@RandomPlan_Set(%s,%s)->arg[0] shouldn't be smaller than 0->%d", __FILE__, __LINE__, arg[0], arg[1], index);
			goto finish;
		}

		if (arg[argnum-1]!=NULL) WarningDx("Warning->@RandomPlan_Set(%s)->Number of arg[trick&percent] > MAX:%d (not Register:%s)", _argument, argnum-3, arg[argnum-1]);

		std::vector <int> numset;
		for (int i=2; i<argnum-1 && arg[i]!=NULL; i++){
			int num;
			if(!( mystrtol(arg[i], &num))){
				ErrorDx("Error->@RandomPlan_Set(%s,%s)->Check argument type[%d]->%s", __FILE__, __LINE__, arg[0], arg[1], i, _argument);
				goto finish;
			}else{
				numset.push_back(num);
			}
		}
		if (numset.size()%2==1) numset.push_back(0);	//引数が奇数の場合、最後尾に0%を追加

		std::vector <std::pair<int,int> > planList;
		for (unsigned int i=0; i<numset.size()/2; i++){
			planList.push_back(std::pair<int,int>(numset[i*2],numset[i*2+1]));
		} 

		_enemySpeciesManager->SetRandomPlanSet(arg[1], index, planList);


//@AI_Set
	}else if (mystrcmp(_command,"@AI_Set")){		
		argnum = 20+2+1;		arg = new char*[argnum];	ArgCut(_command, _argument, arg, argnum, false);	//必須

		if (arg[0]==NULL){
			ErrorDx("Error->@AI_Set->arg[EnemyName]=NULL", __FILE__, __LINE__);
			goto finish;
		}else if (arg[1]==NULL){
			ErrorDx("Error->@AI_Set('%s')->arg[PLANNER_TYPE]=NULL", __FILE__, __LINE__, arg[0]);
			goto finish;
		}
		if (arg[argnum-1]!=NULL) WarningDx("Warning->@AI_Set(%s)->Number of arg[PLANNER_argument] > MAX:%d (not Register:'%s'...)", __FILE__, __LINE__, _argument, argnum-3, arg[argnum-1]);
		
		std::vector <std::string> stringList;
		for (int i=2; i<argnum-1 && arg[i]!=NULL; i++){
			stringList.push_back(arg[i]);
		}

		_enemySpeciesManager->SetEnemyPlanner(arg[0], arg[1], stringList);


//@Encount_Set
	}else if (mystrcmp(_command,"@Encount_Set")){		
		argnum = 3;		arg = new char*[argnum];	ArgCut(_command, _argument, arg, argnum);	//必須

		int num[2];
		for (int i=0; i<2; i++){
			if(!( mystrtol(arg[i], &num[i]))){
				ErrorDx("Error->@Encount_Set->Check argument type->%s", __FILE__, __LINE__, _argument);
				goto finish;
			}
		}
		double dnum;
		if(!( mystrtod(arg[2], &dnum))){
			ErrorDx("Error->@Encount_Set->Check argument type->%s", __FILE__, __LINE__, _argument);
			goto finish;
		}
	
		_enemySpeciesManager->SetMapEncount(num[0], num[1], (int)(dnum*10));	//100% = 1000に変換


//@Party_Set
	}else if (mystrcmp(_command,"@Party_Set")){		
		argnum = MAX_ENEMY_NUM+3+1;		arg = new char*[argnum];	ArgCut(_command, _argument, arg, argnum, false);	//必須

		if (arg[0]==NULL){
			ErrorDx("Error->@Party_Set->arg[mapnum]=NULL", __FILE__, __LINE__);
		}else if (arg[1]==NULL){
			ErrorDx("Error->@Party_Set->arg[chipnum]=NULL", __FILE__, __LINE__);
		}else if (arg[2]==NULL){
			ErrorDx("Error->@Party_Set->arg[encount]=NULL", __FILE__, __LINE__);

		}else{
			int num[3];
			for (int i=0; i<3; i++){
				if(!( mystrtol(arg[i], &num[i]))){
					ErrorDx("Error->@Party_Set->Check argument type->%s", __FILE__, __LINE__, _argument);
					goto finish;
				}
			}
			
			if (arg[argnum-1]!=NULL) WarningDx("Warning->@Party_Set(%s)->Number of arg[enemy] > MAX_ENEMY_NUM:%d", __FILE__, __LINE__, _argument, arg[argnum-1], MAX_ENEMY_NUM);
			
			std::vector <std::string> enemyList;
			
			for (int i=3; i<argnum-1 && arg[i]!=NULL; i++){
				enemyList.push_back(arg[i]);				
			}
			_enemySpeciesManager->AddMapEncountParty(num[0], num[1], num[2], enemyList);
		}

//@BackGround_Set
	}else if (mystrcmp(_command,"@BackGround_Set")){	
		argnum = 3;		arg = new char*[argnum];	if(!ArgCut(_command, _argument, arg, argnum, false))goto finish;	//必須

		if (arg[0]==NULL || arg[1]==NULL){
			ErrorDx("Error->@BackGround_Set->Need more argument:%s", arg[0]);
			goto finish;
		}

		int mapnum;
		if (!mystrtol(arg[0], &mapnum)){
			ErrorDx("Error->Check argument type->%s", __FILE__, __LINE__, _command);
			goto finish;
		}

		if (arg[2]!=NULL){
			int chipnum;
			if (!mystrtol(arg[1], &chipnum)){
				ErrorDx("Error->Check argument type->%s", __FILE__, __LINE__, _command);
				goto finish;
			}
			_bimgbank->SetBattleBackGround(arg[2], mapnum, chipnum);
		}else{
			_bimgbank->SetBattleBackGround(arg[1], mapnum);
		}


//コマンド不一致
	}else{
		return false;
	}

finish:
	delete [] arg;
	return true;
}

//////////////////////////////////////////////////////////////////
//シナリオコマンド（アクションコマンド）の処理////////////////////
//////////////////////////////////////////////////////////////////
bool CCmdManager::FieldCmdSolve(const char* _command, char* _argument, CField* _field, CMap* _map, CTextBox* _textbox, CEveManager* _evemanager){
	int argnum=0;	char** arg;

	if (strlen(_command)==0){
		ErrorDx("Error->strlen(_command)==0->%s", __FILE__, __LINE__, _command);
		return true;
		
//@GameOver
	}else if (mystrcmp(_command, "@GameOver",'p')){
		argnum = 1;		arg = new char*[argnum];	ArgCut(_command, _argument, arg, argnum, false);	//必須
		_field->SetGameMode(MODE_GAMEOVER);
		
//@GameClear
	}else if (mystrcmp(_command, "@GameOver",'p')){
		argnum = 1;		arg = new char*[argnum];	ArgCut(_command, _argument, arg, argnum, false);	//必須
		_field->SetGameMode(MODE_GAMECLEAR);

//@BackToTitle
	}else if (mystrcmp(_command, "@GameOver",'p')){
		argnum = 1;		arg = new char*[argnum];	ArgCut(_command, _argument, arg, argnum, false);	//必須
		_field->SetGameMode(MODE_BACKTOTITLE);

//@BattleResult_Set
	}else if (mystrcmp(_command, "@BattleResult_Set",'p')){
		argnum = 2;		arg = new char*[argnum];	//if(!ArgCut(_command, _argument, arg, argnum))goto finish;	//必須の例外

		//@BattleResult_Set()の引数を手動分解することで、ArgCutの引数誤認を回避
		//@コマンド以外でも分解して渡せる
		char winCommand[256];
		char loseCommand[256];
		char* p;
	
		mystrsmt(_argument, " ,");
		if ((p=strrchr(_argument, '@'))!=NULL && p!=_argument){
				mystrcpy(loseCommand, p);
			*p = '\0';
			mystrsmt(_argument, " ,");
			mystrcpy(winCommand, _argument);
		}else if ((p=strrchr(_argument, ','))!=NULL){
			*p = '\0';
			mystrcpy(winCommand, _argument);
			mystrcpy(loseCommand, ++p);
			mystrsmt(winCommand, " ,");
			mystrsmt(loseCommand, " ,");
		}else{
			ErrorDx("Error->@BattleResult_Set->argErr:%s", _argument);
			goto finish;
		}

		_field->SetBattleResult(winCommand, loseCommand);


//@Battle
	}else if (mystrcmp(_command, "@Battle",'p')){
		argnum = MAX_ENEMY_NUM+1;		arg = new char*[argnum];	ArgCut(_command, _argument, arg, argnum, false);	//必須

		if (arg[0]==NULL || arg[1]==NULL){
			ErrorDx("Error->@Battle->arg[pic_bg] or arg[enemy00] == NULL (do nothing)");
			goto finish;
		}

		std::vector <std::string> enemyList;
		for (int i=1; i<argnum; i++){
			if (arg[i]!=NULL) enemyList.push_back(arg[i]);
		}

		_field->BattleStart(arg[0], enemyList);

//@BattleEncount
	}else if (mystrcmp(_command, "@BattleEncount",'p')){
		argnum = 1;		arg = new char*[argnum];	//ArgCut(_command, _argument, arg, argnum, false);	//必須

		_field->BattleStart();

//@Dir_Set
	}else if (mystrcmp(_command, "@Dir_Set",'l')){
		argnum = 2;		arg = new char*[argnum];	if(!ArgCut(_command, _argument, arg, argnum))goto finish;	//必須

		if( sys::PlayerName(arg[0]) ){
			_field->SetMyDir(sys::StrtoDir(arg[1] ,_textbox->GetOriginalDir()));
		}else{
			_evemanager->SetDir(arg[0], sys::StrtoDir(arg[1] ,_textbox->GetOriginalDir()));
		}

//@Visible_Set
	}else if (mystrcmp(_command, "@Visible_Set",'l')){
		argnum = 2;		arg = new char*[argnum];	if(!ArgCut(_command, _argument, arg, argnum))goto finish;	//必須

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
		argnum = 2;		arg = new char*[argnum];	if(!ArgCut(_command, _argument, arg, argnum))goto finish;	//必須

		int per;
		if (!(mystrtol(arg[1], &per))){
			ErrorDx("Error->@Alpha_Set->Check type arg[per]->%s", __FILE__, __LINE__, arg[1]);
		}else{
			if (per<0 || per>100) {
				ErrorDx("Error->@Alpha_Set-> 0<=|arg[per]|<=100 :%d", per);
			}else{
				if( sys::PlayerName(arg[0]) ){
					_field->SetMyAlpha(per*255/100);
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
			if( sys::PlayerName(arg[0]) ){
				ErrorDx("Error->@Effect_Set->You can't use [PlayerName]with[TMP]:%s",__FILE__, __LINE__, arg[0]);
			}else{
				_evemanager->SetEffect(arg[0], -1, num);		//TextBox.Termからの呼び出し
			}
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

		if( sys::PlayerName(arg[0]) ){
			_field->SetMyEffect(effect, num);
		}else{	
			_evemanager->SetEffect(arg[0], effect, num);
		}

//@Pic_Set
	}else if (mystrcmp(_command, "@Pic_Set",'l')){
		argnum = 2;		arg = new char*[argnum];	if(!ArgCut(_command, _argument, arg, argnum))goto finish;	//必須
		
		if( sys::PlayerName(arg[0]) ){
			_field->SetMyPic(_map->GetImgData(arg[1]), arg[1]);
		}else{		
			_evemanager->SetPic(arg[0], _map->GetImgData(arg[1]), arg[1]);
		}



//@Flag_Create
	}else if (mystrcmp(_command, "@Flag_Create",'l')){
		argnum = 1;		arg = new char*[argnum];	if(!ArgCut(_command, _argument, arg, argnum))goto finish;	//必須
		
		_field->FlagSet.CreateNewFlag(arg[0]);


//@Flag_Set
	}else if (mystrcmp(_command, "@Flag_Set",'l')){
		argnum = 2;		arg = new char*[argnum];	if(!ArgCut(_command, _argument, arg, argnum))goto finish;	//必須

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
		argnum = 2;		arg = new char*[argnum];	if(!ArgCut(_command, _argument, arg, argnum))goto finish;	//必須

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
//		argnum = 1;		arg = new char*[argnum];	if(!ArgCut(_command, _argument, arg, argnum))goto finish;	//必須
//
//		_evemanager->SetNowName(arg[0]);
		
//@Jump
	}else if (mystrcmp(_command, "@Jump",'l')){
		argnum = 1;		arg = new char*[argnum];	if(!ArgCut(_command, _argument, arg, argnum))goto finish;	//必須

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
		cell = between(0, 100, (int)cell);
		
		_textbox->SetReturnVisible(false);

		for (int i=0; i<cell; i++){
			if( sys::PlayerName(arg[0]) ){
				_field->Walk(sys::StrtoDir(arg[1],_textbox->GetOriginalDir()), walkspeed, true, true);
			}else{
				_evemanager->Walk(_field, arg[0], sys::StrtoDir(arg[1],_textbox->GetOriginalDir()), walkspeed, true);
			}
		}
		
		_textbox->SetReturnVisible(true);


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
			cell = between(0, 100, cell);
		}
		
		_textbox->SetReturnVisible(false);

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

		_textbox->SetReturnVisible(true);

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

		_textbox->SetReturnVisible(false);

		if( sys::PlayerName(arg[0]) ){
			_field->Walk(sys::StrtoDir(arg[1],_textbox->GetOriginalDir()), walkspeed, true, true, fade);
		}else{
			_evemanager->Walk(_field, arg[0], sys::StrtoDir(arg[1],_textbox->GetOriginalDir()), walkspeed, true, fade);
		}

		_textbox->SetReturnVisible(true);


//コマンド不一致
	}else{
		return false;
	}
	
finish:
	delete [] arg;
	return true;
}

bool CCmdManager::WindowCmdSolve(const char* _command, char* _argument, CWorldManager* _worldmanager, CMap* _map, CTextBox* _textbox){
	int argnum=0;	char** arg;

	if (strlen(_command)==0){
		ErrorDx("Error->strlen(_command)==0->%s", __FILE__, __LINE__, _command);
		return true;

//@Wait
	}else if (mystrcmp(_command, "@Wait",'l')){
		argnum = 1;		arg = new char*[argnum];	if(!ArgCut(_command, _argument, arg, argnum))goto finish;	//必須

		long int num;
		if(!( mystrtol(arg[0], &num))){
			ErrorDx("Error->Check argument type->%s", __FILE__, __LINE__, _command);
			goto finish;
		}else{
			if (num<0 || num>10000) WarningDx("Warning->@Wait 0<=|arg[time miliseconds]|<=10000: %d", __FILE__, __LINE__, (int)num); 
			num = between(0, 10000, (int)num);
		}

		_worldmanager->Draw(true, true);
		WaitTimer(num);		//一定時間経過するまでwhileループさせてDrawする方が各種アニメーションが止まらないためいいのでは？←必要ならどっちも作っちゃえよ$

//@Anten
	}else if (mystrcmp(_command, "@Anten",'l')){
		argnum = 1;		arg = new char*[argnum];	if(!ArgCut(_command, _argument, arg, argnum))goto finish;	//必須

		long int num;
		if(!( mystrtol(arg[0], &num))){
			ErrorDx("Error->Check argument type->%s", __FILE__, __LINE__, _command);
			goto finish;
		}else{
			if (num<1 || num>10000) WarningDx("Warning->@Anten 0<|arg[time miliseconds]|<=10000: %d", __FILE__, __LINE__, (int)num); 
			num = between(1, 10000, (int)num);
		}

		_worldmanager->FadeDraw(num, BLACK, true, true);
	
//@Meiten
	}else if (mystrcmp(_command, "@Meiten",'l')){
		argnum = 1;		arg = new char*[argnum];	if(!ArgCut(_command, _argument, arg, argnum))goto finish;	//必須

		long int num;
		if(!( mystrtol(arg[0], &num))){
			ErrorDx("Error->Check argument type->%s", __FILE__, __LINE__, _command);
			goto finish;
		}else{
			if (num<1 || num>10000) WarningDx("Warning->@Meiten 0<|arg[time miliseconds]|<=10000: %d", __FILE__, __LINE__, (int)num); 
			num = between(1, 10000, (int)num);
		}

		_worldmanager->FadeDraw(num, BLACK, false, true);
		
//@BackGround_Change
	}else if (mystrcmp(_command, "@BackGround_Change",'l')){
		argnum = 2;		arg = new char*[argnum];	if(!ArgCut(_command, _argument, arg, argnum))goto finish;	//必須

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
			_worldmanager->FadeDraw(num, img, false, false);	
		}else{
			_worldmanager->FadeDraw(num, img, true, false);	
		}

//@Window_Shake
	}else if (mystrcmp(_command, "@Window_Shake",'p')){
		argnum = 2;		arg = new char*[argnum];	if(!ArgCut(_command, _argument, arg, argnum))goto finish;	//必須
		
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
			_worldmanager->Draw(true, true, 0, d, true);
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
			_worldmanager->Draw(true, true, (dir==RIGHT? d: (dir==LEFT? -d: 0)), (dir==DOWN? d: (dir==UP? -d: 0)), true);
		}
		WaitTimer(waittime);
		while (d>0){
			d-=speed;
			_worldmanager->Draw(true, true, (dir==RIGHT? d: (dir==LEFT? -d: 0)), (dir==DOWN? d: (dir==UP? -d: 0)), true);
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
				_worldmanager->Draw(true, true, (dir==RIGHT? d: (dir==LEFT? -d: 0)), (dir==DOWN? d: (dir==UP? -d: 0)), true);
			}

		}else{	//back
			if (!(mystrcmp(arg[3], 'p', 3, "BACK", "Back", "back"))) WarningDx("Warning->@Window_Move->Check arg[option_back]: ", arg[3]);
			int d=cell*MAP_CHIP_SIZE;
			while (d>0){
				d-=speed;
				_worldmanager->Draw(true, true, (dir==RIGHT? -d: (dir==LEFT? d: 0)), (dir==DOWN? -d: (dir==UP? d: 0)), true);
			}
		}

//コマンド不一致
	}else{
		return false;
	}
	
finish:
	delete [] arg;
	return true;
}

bool CCmdManager::TextCmdSolve(const char* _command, char* _argument, CWorldManager* _worldmanager, CTextBox* _textbox){
	int argnum=0;	char** arg;

	if (strlen(_command)==0){
		ErrorDx("Error->strlen(_command)==0->%s", __FILE__, __LINE__, _command);
		return true;
		
//@EventWrap
	}else if (mystrcmp(_command, "@EventWrap",'l')){
		argnum = 1;		arg = new char*[argnum];//	if(!ArgCut(_command, _argument, arg, argnum))goto finish;	//必須のはずがまさかの例外
		char tmp[256];
		sprintf_s(tmp, "@EventWrap(%s)", _argument);	//完全な形でコマンドが必要なため
		_worldmanager->ChangeTextMode(false, tmp);

//@TextMode_Set	//Inner
	}else if (mystrcmp(_command, "@TextMode_Set",'l')){
		argnum = 1;		arg = new char*[argnum];	if(!ArgCut(_command, _argument, arg, argnum))goto finish;	//必須

		if (mystrcmp(arg[0], 'p', 3, "BOX", "Box", "box")){
			_worldmanager->ChangeTextMode(true);
		}else if(mystrcmp(arg[0], 'p', 3, "WRAP", "Wrap", "wrap")){
			_worldmanager->ChangeTextMode(false);
		}else{
			WarningDx("Warning->@TextMode_Set->Check arg[mode]->%s", arg[0]);
			_worldmanager->ChangeTextMode(true);
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

//@Dammy
	}else if (mystrcmp(_command, "@Dammy",'l')){
		argnum = 1;		arg = new char*[argnum];	//ArgCut(_command, _argument, arg, argnum, false);

			
//コマンド不一致
	}else{
		return false;
	}
	
finish:
	delete [] arg;
	return true;
}


//////////////////////////////////////////////////////////////////
//バトルコマンドの処理////////////////////////////////////////////
bool CCmdManager::BattleCmdSolve(const char* _command, char* _argument, CBattle* _battle){
	int argnum=0;	char** arg;

	if (strlen(_command)==0){
		ErrorDx("Error->strlen(_command)==0->%s", __FILE__, __LINE__, _command);
		return true;
		
//@Target_Appear
	}else if (mystrcmp(_command, "@Target_Appear",'l')){
		argnum = 2;		arg = new char*[argnum];	if(!ArgCut(_command, _argument, arg, argnum))goto finish;	//必須

		bool side;
		if (mystrcmp(arg[0], 'p', 3, "ENEMY", "enemy", "Enemy")){
			side = true;
		}else if (mystrcmp(arg[0], 'p', 3, "PLAYER", "player", "Player")){
			side = false;
		}else{
			WarningDx("Warning->@Target_Appear->Check arg[side]...ENEMYorPLAYER", arg[0]);
			side = true;
		}

		int index = 0;
		if (!mystrtol(arg[1], &index)) ErrorDx("Error->@Target_Appear->Check type arg[index]->%s", __FILE__, __LINE__, arg[1]);		

		_battle->TargetMarker.SetVisible(true);
		_battle->TargetMarker.SetSide(side);
		_battle->TargetMarker.SetIndex(index);
		
//@Target_Disappear
	}else if (mystrcmp(_command, "@Target_Disappear",'l')){
		argnum = 1;		arg = new char*[argnum];
		
		_battle->TargetMarker.SetVisible(false);


//@Target_Move
	}else if (mystrcmp(_command, "@Target_Move",'l')){
		argnum = 1;		arg = new char*[argnum];	if(!ArgCut(_command, _argument, arg, argnum))goto finish;	//必須

		_battle->TargetMarker.Move(sys::StrtoDir(arg[0]));


//@Target_Decide
	}else if (mystrcmp(_command, "@Target_Decide",'l')){
		argnum = 2;		arg = new char*[argnum];	if(!ArgCut(_command, _argument, arg, argnum))goto finish;	//必須

		int actorindex = -1;
		if (!mystrtol(arg[0], &actorindex)) ErrorDx("Error->@Target_Decode->Check type arg[index]->%s", __FILE__, __LINE__, arg[0]);
		if (actorindex<0 || actorindex>_battle->GetActorNum()){
			ErrorDx("Error->@Target_Decide-> 0<=arg[actorindex]<ACTOR_NUM :%d", actorindex);
			goto finish;
		}

		bool deadok = sys::TrueOrFalse(arg[1], true);
		
		_battle->TargetMarker.Decide(_battle, actorindex, deadok);


//@Damage
	}else if (mystrcmp(_command, "@Damage",'l')){
		argnum = 4;		arg = new char*[argnum];	if(!ArgCut(_command, _argument, arg, argnum))goto finish;	//必須

		int attacker_actorindex = -1;
		if (!mystrtol(arg[0], &attacker_actorindex)) ErrorDx("Error->@Damage->Check type arg[attacker_actorindex]->%s", __FILE__, __LINE__, arg[0]);
		if (attacker_actorindex<0 || attacker_actorindex>_battle->GetActorNum()){
			ErrorDx("Error->@Damage-> 0<=arg[attacker_actorindex]<ACTOR_NUM :%d", attacker_actorindex);
			goto finish;
		}

		int target_actorindex = -1;
		if (!mystrtol(arg[1], &target_actorindex)) ErrorDx("Error->@Damage->Check type arg[target_actorindex]->%s", __FILE__, __LINE__, arg[1]);
		if (target_actorindex<0 || target_actorindex>_battle->GetActorNum()){
			ErrorDx("Error->@Damage-> 0<=arg[target_actorindex]<ACTOR_NUM :%d", target_actorindex);
			goto finish;
		}

		int trick = NULL;
		if (!mystrtol(arg[2], &trick)) {
			ErrorDx("Error->@Damage->Check type arg[trick]->%s", __FILE__, __LINE__, arg[2]);
			goto finish;
		}

		//変数の準備ができたらいざ処理へ
		if (mystrcmp(arg[3], "NORMAL")){
			_battle->Damage(attacker_actorindex, target_actorindex, (trick_tag const*)trick);

		//}else if (mystrcmp(arg[3], "STABLE")){ 
		}else{
			ErrorDx("Error->@Damange->Check arg[TYPE] :%s", arg[3]);
		}

		//NORMAL, STABLE, PERCENT, NPDEF$
	

//コマンド不一致
	}else{
		return false;
	}
	
finish:
	delete [] arg;
	return true;
}
//////////////////////////////////////////////////////////////////