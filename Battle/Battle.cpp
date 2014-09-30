#include "../Define.h"
#include "Battle.h"
#include "EnemyPlanManager.h"

CBattle::CBattle() : TargetMarker(&ACTOR_NUM){
	
	ACTOR_NUM = 0;
	PLAYER_NUM = 0;
	ENEMY_NUM = 0;
	TextBox = &TextBox1;
	Img_BattleBackGround = NULL;
	
}

void CBattle::Init(){	//Field.Init()�ŌĂяo��	//14/06/26
	//Field.Init�ɂ�����ړ�����Ǝv����
		TrickManager.Clear();

		TrickManager.Add("�A�^�b�N�}�W�b�NA", 110, 3, trick_tag::targetType_tag::SINGLE, 0);
		TrickManager.Add("�A�^�b�N�}�W�b�NB", 8, 2, trick_tag::targetType_tag::SINGLE, 0);

		//trick_tag newtrick;
		//{
		//	strcpy_s(newtrick.Name, "�A�^�b�N�}�W�b�NA");
		//	newtrick.Cost = 3;
		//	newtrick.Power = 110;
		//	newtrick.TargetType = newtrick.SINGLE;
		//	TrickManager.Add(newtrick);
		//}
		//{	strcpy_s(newtrick.Name, "�A�^�b�N�}�W�b�NB");
		//	newtrick.Cost = 2;
		//	newtrick.Power = 8;
		//	newtrick.TargetType = newtrick.SINGLE;
		//	TrickManager.Add(newtrick);
		//}

	//���C���̃e�L�X�g�{�b�N�X�ƃI�[�o�[���b�v�p�e�L�X�g�{�b�N�X�̏�����
		TextBox1.Init(60, 370, WINDOW_WIDTH-80*2, 100, 3, 25*2, 16, WHITE, BLACK, 3);	//�R���X�g���N�^�ɏ�������N�����Ȃ��Ȃ���
		TextWrap1.Init(100, 100, 400, 300, 30, 30*2, 14, WHITE, GRAY, 3);  
		TextBox = &TextBox1;
		
	SetTransColor(255, 0, 255);	//���ߐF�w��	//�ŏI�I�ɂ͓��ꂵ�Ȃ��Ƃȁc$

	//�摜�ǂݍ���
		ImgBank.clear();
		PlayerImgBank.clear();
		EnemyImgBank.clear();

		Img_BattleBackGround = LoadGraph("tenyu_data/pic/sys/battle/background (4).bmp", true);		//������̓R�}���h����ǂݍ���
		ImgBank.insert( std::map<int, int>::value_type( MENU_CURSOR, LoadGraph("tenyu_data/pic/sys/battle/cursor.png",true)) );
		ImgBank.insert( std::map<int, int>::value_type( TIME_BAR1, LoadGraph("tenyu_data/pic/sys/battle/timebar1.png",true)) );
		ImgBank.insert( std::map<int, int>::value_type( TIME_BAR2, LoadGraph("tenyu_data/pic/sys/battle/timebar2.png",true)) );
		ImgBank.insert( std::map<int, int>::value_type( HP_BAR, LoadGraph("tenyu_data/pic/sys/battle/hpbar.png",true)) );

		EnemyImgBank.push_back(LoadGraph("tenyu_data/pic/sys/battle/enemy01.bmp",true));
		EnemyImgBank.push_back(LoadGraph("tenyu_data/pic/sys/battle/enemy02.bmp",true));
		EnemyImgBank.push_back(LoadGraph("tenyu_data/pic/sys/battle/enemy03.bmp",true));
		
		PlayerImgBank.push_back(LoadGraph("tenyu_data/pic/sys/battle/player.bmp",true));
		PlayerImgBank.push_back(LoadGraph("tenyu_data/pic/sys/battle/player.bmp",true));
		PlayerImgBank.push_back(LoadGraph("tenyu_data/pic/sys/battle/player.bmp",true));


	//�^�[�Q�b�g�I���}�[�J�[������
		TargetMarker.Init(LoadGraph("tenyu_data/pic/sys/battle/target.png", true));
	
	SetTransColor(0, 0, 0);	//���ߐF�w��

	//PlayerSpecies�̐����i�e�X�g�p�j
		PlayerSpeciesManager.CreateSpecies("�v���C���[A", 100, 5, 5, 1);
		PlayerSpeciesManager.CreateSpecies("�v���C���[B", 100, 5, 5, 1);
		PlayerSpeciesManager.CreateSpecies("�v���C���[C", 100, 5, 5, 1);
		PlayerSpeciesManager.CreateSpecies("�v���C���[D", 100, 5, 5, 1);
	
	//PlayerSpecies�̋Z���X�g�Z�b�g�i�e�X�g�p�j
		std::vector <trick_tag const*>p_trickList;
		p_trickList.push_back(TrickManager.GetTrick("�A�^�b�N�}�W�b�NA"));
		p_trickList.push_back(TrickManager.GetTrick("�A�^�b�N�}�W�b�NB"));
		PlayerSpeciesManager.SetTrickList("�v���C���[A", p_trickList);
		PlayerSpeciesManager.SetTrickList("�v���C���[B", p_trickList);
		PlayerSpeciesManager.SetTrickList("�v���C���[C", p_trickList);
		PlayerSpeciesManager.SetTrickList("�v���C���[D", p_trickList);

	//PlayerSpecies��Img�Z�b�g�i�e�X�g�p�j
		PlayerSpeciesManager.SetImg("�v���C���[A", PlayerImgBank[0]);
		PlayerSpeciesManager.SetImg("�v���C���[B", PlayerImgBank[1]);
		PlayerSpeciesManager.SetImg("�v���C���[C", PlayerImgBank[2]);
		PlayerSpeciesManager.SetImg("�v���C���[D", PlayerImgBank[3]);

	//EnemySpecies�̐����i�e�X�g�p�j
		EnemySpeciesManager.CreateSpecies("�G�l�~�[A", 100, 5, 5, 1);
		EnemySpeciesManager.CreateSpecies("�G�l�~�[B", 100, 5, 5, 1);
		EnemySpeciesManager.CreateSpecies("�G�l�~�[C", 100, 5, 5, 1);
		EnemySpeciesManager.CreateSpecies("�G�l�~�[D", 100, 5, 5, 1);
	
	//EnemySpecies�̋Z���X�g�Z�b�g�i�e�X�g�p�j
		std::vector <trick_tag const*>e_trickList;
		e_trickList.push_back(TrickManager.GetTrick("�A�^�b�N�}�W�b�NA"));
		e_trickList.push_back(TrickManager.GetTrick("�A�^�b�N�}�W�b�NB"));
		EnemySpeciesManager.SetTrickList("�G�l�~�[A", e_trickList);
		EnemySpeciesManager.SetTrickList("�G�l�~�[B", e_trickList);
		EnemySpeciesManager.SetTrickList("�G�l�~�[C", e_trickList);
		EnemySpeciesManager.SetTrickList("�G�l�~�[D", e_trickList);

	//EnemySpecies��Img�Z�b�g�i�e�X�g�p�j
		EnemySpeciesManager.SetImg("�G�l�~�[A", EnemyImgBank[0]);
		EnemySpeciesManager.SetImg("�G�l�~�[B", EnemyImgBank[1]);
		EnemySpeciesManager.SetImg("�G�l�~�[C", EnemyImgBank[2]);
		EnemySpeciesManager.SetImg("�G�l�~�[D", EnemyImgBank[3]);
}

void CBattle::SetPlayer(const int _playerNum, ...){
	va_list args;
	va_start( args, _playerNum);	//target���傫�������Ƃ��̏��u���@�͂Ȃ��̂��H
	
	if (_playerNum<=0){
		ErrorDx("Error->arg[playerNum] should >=1: playerNum=%d", __FILE__, __LINE__, _playerNum);
	}else{		
		PLAYER_NUM = _playerNum;
		Player = new CPlayer[PLAYER_NUM];
		for (int i=0; i<PLAYER_NUM; i++){
			Player[i] = CPlayer(PlayerSpeciesManager.GetSpecies(va_arg(args, char*)));		//target=1�̎��A��ڂ�Ԃ��iNot target=0�j
		}
		va_end(args);
	}
}

void CBattle::SetEnemy(const int _enemyNum, ...){
	va_list args;
	va_start( args, _enemyNum);	//target���傫�������Ƃ��̏��u���@�͂Ȃ��̂��H
	
	if (_enemyNum<=0){
		ErrorDx("Error->arg[enemyNum] should >=1: enemyNum=%d", __FILE__, __LINE__, _enemyNum);
	}else{		
		ENEMY_NUM = _enemyNum;
		Enemy = new CEnemy[ENEMY_NUM];
		for (int i=0; i<ENEMY_NUM; i++){
			Enemy[i] = CEnemy(EnemySpeciesManager.GetSpecies(va_arg(args, char*)));		//target=1�̎��A��ڂ�Ԃ��iNot target=0�j
		}
		va_end(args);
	}
}

void CBattle::BattleStart(int* _result, CFlagSet* _flagset_p, CField* _field_p, CMap* _map_p, CEveManager* _evemanager_p){
	//�J�n����///////////////////////////////////////////////////
		
		//Player����
			SetPlayer(3, "�v���C���[A", "�v���C���[B", "�v���C���[C");

		//Enemy����
			DebugDx("SetEnemy_Start:%d", ENEMY_NUM);
			SetEnemy(3, "�G�l�~�[A", "�G�l�~�[B", "�G�l�~�[C");
			DebugDx("SetEnemy_FIN:%s", Enemy[0].GetName().c_str());

		//Actor�ւ������ݸނ�TextBox�ւ̕R�t��
			//Actor��new�Ńo�g�����Ƃɐ���
			ACTOR_NUM = PLAYER_NUM + ENEMY_NUM;
			Actor = new CActor*[ACTOR_NUM];
			for (int i=0; i<ACTOR_NUM; i++){
				Actor[i] = ((i<MAX_PLAYER)? (CActor*)&Player[i]: (CActor*)&Enemy[i-MAX_PLAYER]);
				Actor[i]->FirstSet(i, &TextBox, &B_CmdList, &ImgBank);
			}

		//�����l�ݒ�
			for (int i=0; i<PLAYER_NUM; i++){	//$�G�ȃe�X�g�p�����l�ݒ�
				Player[i].CreateBattleMenu();//���K��AddTrick�̌�ɂ��邱�Ɓi������BattleMenu������Ă��邽�߁j
				Player[i].SetRect(WINDOW_WIDTH/4*(i+1), WINDOW_HEIGHT-200);
			}
			
			CEnemyPlanManager::GetInstance()->SetActor_p(Actor);	//EnemyPlanManager�͂��ׂĂ�Actor�ւ̃|�C���^������
			CEnemyPlanManager::GetInstance()->Init();
			for (int i=0; i<ENEMY_NUM; i++){
				Enemy[i].SetEnemyPlanManager(CEnemyPlanManager::GetInstance());
				Enemy[i].MakePlan();

				Enemy[i].SetRect(WINDOW_WIDTH/4*(i+1), 70);
			}


		EveManager_p = _evemanager_p;
		FlagSet_p = _flagset_p;
		B_CmdManager.Init(_map_p);


		TextBox->AddStock("�퓬�J�n�I");/*
		TextBox->AddStock("@EventWrap(wrap_sample, [�퓬])");
		TextBox->AddStock("@Anten(1000)");
		TextBox->AddStock("@Meiten(2000)");
		TextBox->AddStock("@Window_Shake(30,5)");*/
		TextBox->NextPage(&B_CmdList, FlagSet_p);


	//���C�����[�v����///////////////////////////////////////////////////
		int tmp_result;
		tmp_result = MainLoop();

	//�I������///////////////////////////////////////////////////
		BattleFinish();
		*_result = tmp_result;
}

int CBattle::MainLoop(){	//�퓬���͂��̃��[�v������o�Ȃ�

	DebugDx("MainLoopStart");
	
	while(BasicLoop()){
		
		if( !TextBox->Main(&B_CmdList, FlagSet_p)){	//�e�L�X�g�\�����̓L�[���얳���i�e�L�X�g�����TextBox.Main�Ŕ���j		

			if (ActionQueue.empty()){	//�s���ҋ@���X�g���󂩃`�F�b�N
			
				//$/////////////////////////////////////////////////////////
					if (CheckHitKeyDown(KEY_INPUT_W)) return WIN;
					if (CheckHitKeyDown(KEY_INPUT_L)) return LOSE;
				////////////////////////////////////////////////////////////

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
		}

		////TextBox�Ȃǂɂ����CmdList�ɒ~�ς��ꂽ�R�}���h������////////////////////////////////////////
			B_CmdManager.Main(&B_CmdList, this, TextBox);

		////�`��////////////////////////////////////////
			Draw();
	}
	return -1;
}

void CBattle::BattleFinish(){
	
	delete [] Actor;
	delete [] Player;
	delete [] Enemy;

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
		for (int i=0; i<MAX_PLAYER; i++){
			Player[i].Draw(dx, dy);
		}
		for (int i=0; i<MAX_ENEMY; i++){
			Enemy[i].Draw(dx, dy);
		}

		//�^�[�Q�b�g�}�[�J�[�̕`��//////////////////////////////////////////////
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

void CBattle::Damage(int _attacker_actorindex, int _target_actorindex, trick_tag const* _trick){
	int attacker_actorindex = between(0, ACTOR_NUM-1, _attacker_actorindex); 
	int target_actorindex   = between(0, ACTOR_NUM-1, _target_actorindex); 
	
	int damage = Actor[target_actorindex]->Damage(Actor[attacker_actorindex], _trick);

	int timecount = 0;
	CRect tmpRect = Actor[target_actorindex]->GetRect();
	while(timecount<20){
		Draw();
		if (timecount<1){	DrawCenterString((int)(tmpRect.Center().x), (int)(tmpRect.Top-5*sin(timecount*(PI/2)/10)), WHITE, "%d", damage); 
		}else{				DrawCenterString((int)(tmpRect.Center().x), tmpRect.Top-5								 , WHITE, "%d", damage); 
		}
		timecount++;
		if (!BasicLoop()) break;
	}

	while(true){
		Draw();
		if (!BasicLoop() || Actor[target_actorindex]->DeadCheck()) break;
	}
}

void CBattle::CTargetMarker::Draw(int dx, int dy){
	if (Visible){
		if(EnemySide){
			DrawGraph(WINDOW_WIDTH/4*(Index+1)+dx,  20+dy, Img, true);
		}else{
			DrawGraph(WINDOW_WIDTH/4*(Index+1)+dx,  WINDOW_HEIGHT-290+dy, Img, true);			
		}
	}
}

void CBattle::CTargetMarker::Move(int _dir){
	switch (_dir){
	case RIGHT:
		Index = mod(Index+1,(EnemySide? MAX_ENEMY: MAX_PLAYER));
		break;
	case LEFT:
		Index = mod(Index-1,(EnemySide? MAX_ENEMY: MAX_PLAYER));
		break;
	default:
		break;
	}

}

void CBattle::CTargetMarker::Decide(CBattle* _battle, int _actorindex, bool _deadok){
	int actorindex = between(0, (*ActorNum_p)-1, _actorindex); 

	if (!_deadok && _battle->Actor[Index + (EnemySide?MAX_PLAYER:0)]->GetHp()==0){
		return;
	}else{
		_battle->Actor[actorindex]->SetTarget(Index + (EnemySide?MAX_PLAYER:0));
		SetVisible(false);
	}

}


