#include "../Define.h"
#include "Battle.h"
#include "EnemyPlanManager.h"
#include "BattleCalculator.h"

#include "../Field/Load.h"

CBattle::CBattle(){
	Ready = false;

	ACTOR_NUM = 0;
	PLAYER_NUM = 0;
	ENEMY_NUM = 0;
	TextBox = &TextBox1;
	Img_BattleBackGround = NULL;

	WinCommand[0] = '\0';
	mystrcpy(LoseCommand, "@GameOver");
}


bool CBattle::Init(){	//Field.Init()�ŌĂяo��	//14/06/26
	PlayerSpeciesManager.Clear();
	EnemySpeciesManager.Clear();
	BImgBank.Init();
	TrickManager.Clear();
	
	ACTOR_NUM = 0;
	PLAYER_NUM = 0;
	ENEMY_NUM = 0;
	
	SetTransColor(255, 0, 255);	//���ߐF�w��	//�ŏI�I�ɂ͓��ꂵ�Ȃ��Ƃȁc$
	
	//Load.cpp��ʂ���.rpg�̓ǂݍ���/////////////////////
	
		//�O���e�L�X�g�̃��[�h
			CLoad bfsLoad;
			CBattleFirstSetCmdManager  bfsCmdManager;
			CCmdList bfsCmdList;
			
			if (bfsLoad.LoadAddText("tenyu_data/b_system.rpg")){			
				bfsLoad.CommandCopy(&bfsCmdList);
				bfsCmdManager.Main(&bfsCmdList, &BImgBank, &PlayerSpeciesManager, &EnemySpeciesManager, &TrickManager);
			}else{
				return false;
			}
	////////////////////////////////////////////////

	//���C���̃e�L�X�g�{�b�N�X�ƃI�[�o�[���b�v�p�e�L�X�g�{�b�N�X�̏�����
		TextBox1.Init(60, 370, WINDOW_WIDTH-80*2, 100, 3, 25*2, 16, WHITE, BLACK, 3);	//�R���X�g���N�^�ɏ�������N�����Ȃ��Ȃ���
		TextWrap1.Init(100, 100, 400, 300, 30, 30*2, 14, WHITE, GRAY, 3);  
		TextBox = &TextBox1;
	
	//�ǂ��ɏ����ׂ������Field?///////
		PlayerSpeciesManager.SetMemberList();
	//////////////////

	SetTransColor(0, 0, 0);	//���ߐF�w��
	return true;
}
void CBattle::Term(){	//�^�C�g���ɖ߂�Ƃ���~CField()����Ăяo��

}
void CBattle::BattleReady(CFlagSet* _flagset_p, CMap* _map_p, CEveManager* _evemanager_p){
	//�퓬���Ƃɍs���J�n������������///////////////////////////////////////////////////	
		
		//Actor�ւ������ݸނ�TextBox�ւ̕R�t��
			ACTOR_NUM = PLAYER_NUM + ENEMY_NUM;
			Actor = new CActor*[ACTOR_NUM];
			for (int i=0; i<ACTOR_NUM; i++){
				Actor[i] = ((i<PLAYER_NUM)? (CActor*)&Player[i]: (CActor*)&Enemy[i-PLAYER_NUM]);
				Actor[i]->FirstSet(PLAYER_NUM, ENEMY_NUM, i, &TextBox, &B_CmdList);
				Actor[i]->SetSystemImg(&BImgBank);
			}

		//�����l�ݒ�
			for (int i=0; i<PLAYER_NUM; i++){
				Player[i].CreateBattleMenu();//���K��AddTrick�̌�ɂ��邱�Ɓi������BattleMenu������Ă��邽�߁j
				Player[i].SetRect(WINDOW_WIDTH/4*(i+1), WINDOW_HEIGHT-200);
			}
			
			CEnemyPlanManager::GetInstance()->SetActor_p(Actor);	//EnemyPlanManager�͂��ׂĂ�Actor�ւ̃|�C���^������
			CEnemyPlanManager::GetInstance()->Init(PLAYER_NUM, ENEMY_NUM);
			for (int i=0; i<ENEMY_NUM; i++){
				Enemy[i].SetEnemyPlanManager(CEnemyPlanManager::GetInstance());
				Enemy[i].MakePlan();

				Enemy[i].SetRect(WINDOW_WIDTH/4*(i+1), 70);
			}

		//�^�[�Q�b�g�I���}�[�J�[������
			SetTransColor(255, 0, 255);	//���ߐF�w��
			TargetMarker.Init(ACTOR_NUM, PLAYER_NUM, ENEMY_NUM, LoadGraph("tenyu_data/pic/sys/battle/target.png", true));
			SetTransColor(0, 0, 0);	//���ߐF�w��

		EveManager_p = _evemanager_p;
		FlagSet_p = _flagset_p;
		B_CmdManager.Init(_map_p);

		Ready = true;
}

void CBattle::BattleStart(int* _result, CCmdList* _fieldcmdlist_p){
	if (!Ready){
		ErrorDx("Error->BattleStart->Battle isn't Ready to Start.", __FILE__, __LINE__);
		*_result = -1;
	}else{

		//�J�n���o//////////////////////////////////////////////////////////
			StartEffect();
		///////////////////////////////////////////////////////////////////

		//���C�����[�v����///////////////////////////////////////////////////
			int tmp_result = MainLoop();

		//�I������///////////////////////////////////////////////////
			BattleFinish(tmp_result, _fieldcmdlist_p);
			*_result = tmp_result;
	}
}
void CBattle::BattleSetting(const char* _wincommand, const char* _losecommand){
	mystrcpy(WinCommand, _wincommand);
	mystrcpy(LoseCommand, _losecommand);
}

bool CBattle::CheckEncount(int _mapnum, int _chipnum){
	std::vector <CEnemySpecies*> tmpparty;

	if (EnemySpeciesManager.CheckEncount(_mapnum, _chipnum, tmpparty)){

		for (unsigned int i =0; i<tmpparty.size(); i++){		
			DebugDx("enemy:%s", tmpparty[i]->GetName().c_str());
		}

		SetEnemy(tmpparty);
		SetBackGround(_mapnum, _chipnum);
		return true;
	}else{
		return false;
	}
}

void CBattle::SetBackGround(const char* _pickey){
	Img_BattleBackGround = BImgBank.GetImg(_pickey);
}
void CBattle::SetBackGround(int _mapnum, int _chipnum){
	Img_BattleBackGround = BImgBank.GetBattleBackGround(_mapnum, _chipnum);
}
void CBattle::SetPlayer(){	//����Ɋ�Â��đI�o
	PLAYER_NUM = min(PlayerSpeciesManager.GetMemberListSize(), MAX_PLAYER_NUM);
	if (PLAYER_NUM==0){
		ErrorDx("Error->Battle::SetPlayer->PLAYER_NUM=0.  SetPartyBattleMember!", __FILE__, __LINE__);
		return;
	}
	Player = new CPlayer[PLAYER_NUM];
	for (int i=0; i<PLAYER_NUM; i++){
		Player[i] = CPlayer(*(PlayerSpeciesManager.GetSpecies(i)));
	}
}
void CBattle::SetPlayer(const int _playerNum, ...){	//�p�[�e�B���R�w��p�i�C�x���g�퓬�j
	PLAYER_NUM=0;
	va_list args;
	va_start( args, _playerNum);	//target���傫�������Ƃ��̏��u���@�͂Ȃ��̂��H
	
	if (_playerNum<=0){
		ErrorDx("Error->arg[playerNum] should >=1: playerNum=%d", __FILE__, __LINE__, _playerNum);
	}else{		
		PLAYER_NUM = _playerNum;
		Player = new CPlayer[PLAYER_NUM];
		for (int i=0; i<PLAYER_NUM; i++){
			Player[i] = CPlayer(*(PlayerSpeciesManager.GetSpecies(va_arg(args, char*))));
		}
		va_end(args);
	}
}
void CBattle::SetEnemy(const int _enemyNum, ...){	//�o�������X�^�[���R�w��p
	va_list args;
	va_start( args, _enemyNum);	//target���傫�������Ƃ��̏��u���@�͂Ȃ��̂��H
	
	if (_enemyNum<=0){
		ErrorDx("Error->arg[enemyNum] should >=1: enemyNum=%d", __FILE__, __LINE__, _enemyNum);
	}else{
		std::vector<std::string> enemyList;
		for (int i=0; i<_enemyNum; i++){
			enemyList.push_back(va_arg(args, char*));
		}
		va_end(args);
	}
}
void CBattle::SetEnemy(std::vector<std::string> _enemyList){
	if (_enemyList.size()==0) ErrorDx("Error->Battle::SetEnemy->enemyNum=0 :%d", __FILE__, __LINE__, _enemyList.size());

	ENEMY_NUM = _enemyList.size();
	Enemy = new CEnemy[ENEMY_NUM];
	for (int i=0; i<ENEMY_NUM; i++){
		Enemy[i] = CEnemy(*(EnemySpeciesManager.GetSpecies(_enemyList[i].c_str())));
	}
}
void CBattle::SetEnemy(std::vector<CEnemySpecies*> _enemyParty){
	if (_enemyParty.size()==0) ErrorDx("Error->Battle::SetEnemy->enemyNum=0 :%d", __FILE__, __LINE__, _enemyParty.size());

	ENEMY_NUM = _enemyParty.size();
	Enemy = new CEnemy[ENEMY_NUM];
	for (int i=0; i<ENEMY_NUM; i++){
		Enemy[i] = CEnemy(*(_enemyParty[i]));
	}
}

void CBattle::StartEffect(){	//�퓬�J�n���o

	char enemyName[256];
	enemyName[0]='\0';
	for (int i=0; i<ENEMY_NUM; i++){
		mystrcat(enemyName, Enemy[i].GetName().c_str());
	}

	int x=WINDOW_WIDTH+20;
	int timecount=0;
	CRect obi(0, WINDOW_WIDTH, 230, 230); 

	do{
		SetTitle("%s", enemyName);
		
		Draw();
	
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
		DrawBox(obi.Left, obi.Top, obi.Right, obi.Bottom, BLACK, true);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
		DrawCenterString(x, (int)(obi.Center().y), WHITE, true, "���I�₹����%s�������ꂽ�I", enemyName);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);

		if (timecount<13) {obi.SetHeight(timecount*4);}
		else if (timecount>100) {obi.SetHeight(max(0,obi.Height()-5));}

		if (x>WINDOW_WIDTH/2+160) {x-=10;}
		else if(x<WINDOW_WIDTH/2-160) {x-=18;}
		else{ x-=4;}

		++timecount;
		if (timecount==120) break;


	}while(BasicLoop());

}

int CBattle::MainLoop(){	//�퓬���͂��̃��[�v������o�Ȃ�
	int result;
	
	do{		
		if( !TextBox->Main(&B_CmdList, FlagSet_p)){	//�e�L�X�g�\�����̓L�[���얳���i�e�L�X�g�����TextBox.Main�Ŕ���j		

			if (ActionQueue.empty()){	//�s���ҋ@���X�g���󂩃`�F�b�N
			
				for (int i=0; i<ACTOR_NUM; i++){
					if (Actor[i]->Main()) ActionQueue.push(Actor[i]);		//TimeBar�i�܂��ĕK�v�Ȃ�s���ҋ@���X�g�ɓ����
				}

			}else{
				if (ActionQueue.front()->Do()){
					ActionQueue.pop();
				}else{
					//�s���ҋ@���X�g�퓬�̃L�����̍s�����������Ȃ������Ƃ��ɂ͂Ȃɂ����Ȃ�
				}
			}

			if ((result=ResultCheck())!=-1){
				B_CmdManager.Main(&B_CmdList, this, TextBox);
				Draw();
				return result;
			}

		}

		////TextBox�Ȃǂɂ����CmdList�ɒ~�ς��ꂽ�R�}���h������////////////////////////////////////////
			B_CmdManager.Main(&B_CmdList, this, TextBox);

		////�`��////////////////////////////////////////
			Draw();
	}while(BasicLoop());

	return -1;
}

void CBattle::BattleFinish(int _result, CCmdList* _fieldcmdlist){
	if (!Ready) {WarningDx("Warning->BattleFinish->Battle hasn't started yet.(do nothing return)", __FILE__, __LINE__); return;}
	Ready = false;
	while(!ActionQueue.empty()) {ActionQueue.pop();}
	B_CmdList.Clear();
	////////////////////////////////////////////////////////////
	
	switch (_result){
	case WIN:
		//���O�o�^�����R�}���h���t�B�[���h�̃R�}���h���X�g�ɑ}��
			if (strlen(WinCommand)) _fieldcmdlist->Add(WinCommand);

		//���ƌo���l�v�Z
			{int gold = BattleCalc::CalcGold(1, 2);
			int exp = BattleCalc::CalcExp(1, 2);
			PlayerSpeciesManager.AddGold(gold);
			//PlayerSpeciesManager.AddExp(exp);
			char text[256];
			sprintf_s(text, "�擾Gold�F%d", gold); TextBox->AddStock(text);
			sprintf_s(text, "�擾Exp�F%d", exp);	TextBox->AddStock(text);
			}

		//Player���f�[�^�ɕۑ�
			PlayerSpeciesManager.CopyValue(PLAYER_NUM, Player);	//PlayerSpecies�z��œn���������L���X�g�ł̓������z�u�̊֌W��z��ł̎Q�Ƃ�����邽�ߒf�O

		break;
	case LOSE:
		if (strlen(LoseCommand)) _fieldcmdlist->Add(LoseCommand);
		break;
	default:
		break;
	}

	int i=0;
	while(BasicLoop()){
		if( !TextBox->Main(&B_CmdList, FlagSet_p)) break;
		B_CmdManager.Main(&B_CmdList, this, TextBox);
		Draw();
	}

	Draw();	//����ʂɕ`��i��ʐ؂�ւ����o�p�j

	//////////////////////////////////////////////////////
	delete [] Actor;
	delete [] Player;
	delete [] Enemy;

	ACTOR_NUM = 0;
	ENEMY_NUM = 0;
	PLAYER_NUM = 0;

	WinCommand[0] = '\0';
	mystrcpy(LoseCommand, "@GameOver");
}

void CBattle::Draw(bool _screenflip, bool _textshowingstop, int dx, int dy, bool _playeralsoshake){
	
	if (ImgBackGround!=NULL){	//�w�i�ꖇ�G���[�h�̂Ƃ�
		CVector picsize = GetGraphSize(ImgBackGround);
		DrawGraph(WINDOW_WIDTH/2-picsize.x/2, WINDOW_HEIGHT/2-picsize.y/2, ImgBackGround, true);
	}else{

		//�w�i�`��////////////////////////////////////////////////////////////
		SetDrawBright(200, 200, 200);
		DrawExtendGraph(0,0,WINDOW_WIDTH, WINDOW_HEIGHT, Img_BattleBackGround,true);
		SetDrawBright(255,255,255);
		/////////////////////////////////////////////////////////////////////

		//Player��Enemy�̕`��///////////////////////////////////////////////////////////////
		for (int i=0; i<ACTOR_NUM; i++){
			Actor[i]->Draw(dx, dy);
		}

		//�^�[�Q�b�g�}�[�J�[�̕`��//////////////////////////////////////////////
		SetDrawBright(255,255,255);
		TargetMarker.Draw(dx,dy);

	}

	////////////////////////////////////////////////////////////////////////////////////////
	//�e�L�X�g�{�b�N�X�`��//////////////////////////////////////////////////////////////////
	TextBox->Draw(_textshowingstop);
	////////////////////////////////////////////////////////////////////////////////////////
	

	///�e�X�g�p//////////////////////////////////////////////////////////////////////////
	SetDrawBlendMode(DX_BLENDMODE_ALPHA , 150+60*sin(2*PI*GetNowCount()/((double)2000)));
	DrawCenterString(WINDOW_WIDTH/2,WINDOW_HEIGHT/2,"�퓬��---W�܂���L�ŏI��",RED);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
	///////////////////////////////////////////////////////////////////////////////////////

	if (_screenflip)	{BasicLoop();}
}

int CBattle::ResultCheck(){
	//$/////////////////////////////////////////////////////////
		if (CheckHitKeyDown(KEY_INPUT_W)) return WIN;
		if (CheckHitKeyDown(KEY_INPUT_L)) return LOSE;
	////////////////////////////////////////////////////////////

	int i;
	for (i=0; i<PLAYER_NUM; i++){
		if (Player[i].GetAlive()) break;
	}
	if (i==PLAYER_NUM) return LOSE;

	for (i=0; i<ENEMY_NUM; i++){
		if (Enemy[i].GetAlive()) break;
	}
	if (i==ENEMY_NUM) return WIN;

	return -1;
}

void CBattle::Damage(int _attacker_actorindex, int _target_actorindex, trick_tag const* _trick){
	int attacker_actorindex = between(0, ACTOR_NUM-1, _attacker_actorindex); 
	int target_actorindex   = between(0, ACTOR_NUM-1, _target_actorindex); 
	
	//�Z�������o//////////////////////////////////////////////////////////////
	int timecount = 0;
	CVector ball[8];
	int a=60;
	for (int i=0; i<8; i++){
		ball[i].Set(Actor[attacker_actorindex]->GetRect().Center().Add(a*cos(i*PI/4),a*sin(i*PI/4)));		
	}
	do{
		Draw();
		SetDrawBlendMode(DX_BLENDMODE_ADD, timecount*2);
		for (int i=0; i<8; i++){
			DrawExtendGraph(ball[i].x-10, ball[i].y-10, ball[i].x+10, ball[i].y+10, BImgBank.GetImg("EFFECT_BOMB"), false);
		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		++timecount;
		a-=1;
		for (int i=0; i<8; i++){
			ball[i].Set(Actor[attacker_actorindex]->GetRect().Center().Add(a*cos(timecount*PI/(60*2)+i*PI/4),a*sin(timecount*PI/(60*2)+i*PI/4)));		
		}
	}while(a!=0 && BasicLoop());

	timecount=0;
	a=20;
	CVector vec = Actor[target_actorindex]->GetRect().Center()-Actor[attacker_actorindex]->GetRect().Center();
	vec *= 1/vec.GetLength();
	do{
		Draw();
		SetDrawBlendMode(DX_BLENDMODE_ADD, 120);
		DrawExtendGraph(ball[0].x-15, ball[0].y-15, ball[0].x+15, ball[0].y+15, BImgBank.GetImg("EFFECT_BOMB"), false);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		++timecount;
		ball[0].Add(a*vec.x, a*vec.y);
	}while(ball[0].y>Actor[target_actorindex]->GetRect().Center().y && BasicLoop());
	//////////////////////////////////////////////////////////////////////////

	timecount=0;
	do{Draw(); if(++timecount==10){break;}}while(BasicLoop());
	
	//���ۂ̃_���[�W�v�Z
	int damage = Actor[target_actorindex]->Damage(Actor[attacker_actorindex], _trick);

	//�_���[�W���o//////////////////////////////////////////////////////////////
	timecount = 0;
	bool oldVisible = Actor[target_actorindex]->GetVisible();
	CVector charPos(Actor[target_actorindex]->GetRect().Center().x, Actor[target_actorindex]->GetRect().Top);
	do{
		Draw();
		DrawCenterString((int)(charPos.x), (int)(charPos.y-5*sin(min(timecount,10)*(PI/2)/10)), WHITE, "%d", damage); 
		
		if (timecount%6==0) Actor[target_actorindex]->SetVisible(oldVisible&&true);
		if (timecount%6==3) Actor[target_actorindex]->SetVisible(oldVisible&&false);

		if (++timecount>40){
			Actor[target_actorindex]->SetVisible(oldVisible);
			break;
		}
	}while(BasicLoop());
	////////////////////////////////////////////////////////////////////////////


	//HP�o�[������҂�///////////////
	while(true){
		Draw();
		if (!BasicLoop() || Actor[target_actorindex]->DeadCheck()) break;
	}
	/////////////////////////////////
}


//////////////////////////////////////////////////////////////
void CBattle::CTargetMarker::Init(int _actornum, int _playernum, int _enemynum, int _img){
	ACTOR_NUM = _actornum;
	PLAYER_NUM = _playernum;
	ENEMY_NUM = _enemynum;
	Img = _img;
	Visible = false;
	EnemySide = true;
	Index = 0;
	Status=0;
}
void CBattle::CTargetMarker::Draw(int dx, int dy){
	if (Visible){
		SetDrawBlendMode(DX_BLENDMODE_ALPHA , 200);
		static int timecount=0;
		timecount++;

		if (Status==0){
			if (timecount==60) timecount=0;
			dy += (int)(5*sin((timecount)*2*PI/60));
		}else if(Status==1){
			timecount=0; Status=2;
		}else if(Status==2){
			if (timecount>30){ Status=0; Visible=false; timecount=0;}
		}
		
		if (!(Status==2 && timecount%10<6)){
			if(EnemySide){
				DrawGraph(WINDOW_WIDTH/4*(Index+1)+dx,  20+dy, Img, true);
			}else{
				DrawGraph(WINDOW_WIDTH/4*(Index+1)+dx,  WINDOW_HEIGHT-290+dy, Img, true);			
			}
		}

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
	}
}

void CBattle::CTargetMarker::Move(int _dir){
	if (Status==1) return;	//�o�O�\�h
	switch (_dir){
	case RIGHT:
		Index = mod(Index+1,(EnemySide? ENEMY_NUM: PLAYER_NUM));
		break;
	case LEFT:
		Index = mod(Index-1,(EnemySide? ENEMY_NUM: PLAYER_NUM));
		break;
	default:
		break;
	}

}

void CBattle::CTargetMarker::Decide(CBattle* _battle, int _actorindex, bool _deadok){
	int actorindex = between(0, ACTOR_NUM-1, _actorindex); 

	if (!_deadok && _battle->Actor[Index + (EnemySide?PLAYER_NUM:0)]->GetHp()==0){
		return;
	}else{
		_battle->Actor[actorindex]->SetTarget(Index + (EnemySide?PLAYER_NUM:0));
		Status = 1;
		do {
			_battle->Draw();	//�J�[�\���m��_�ŉ��o
		}while(BasicLoop()&&Status!=0);
	}

}

void CBattle::ChangeTextMode(bool _box, const char* _eventtext){
	if (_box){
		TextBox = &TextBox1;
	}else{
		TextBox = &TextWrap1;

		if (_eventtext!=NULL){	//EveManager::CopyOriginalEvent��ėp�����グ�ĉ��P�B�����TextWrap1��@EventWrap�̓��e��n����
			std::vector<char256> tmptext;
			EveManager_p->CopyOriginalEvent(&tmptext, _eventtext);
			for (unsigned int i=0; i<tmptext.size(); i++){
				TextWrap1.AddStock(tmptext[i].text);
			}
			TextBox->NextPage(&B_CmdList, FlagSet_p);
		}
	}
};
