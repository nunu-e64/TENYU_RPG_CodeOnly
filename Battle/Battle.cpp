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

void CBattle::Init(){	//Field.Init()で呼び出す	//14/06/26

	CBattleFirstSetCmdManager  bfsCmdManager;
	CCmdList bfsCmdList;

	TrickManager.Clear();
	
	SetTransColor(255, 0, 255);	//透過色指定	//最終的には統一しないとな…$
	
	//Load.cppを通して.rpgの読み込み/////////////////////$
		bfsCmdList.Add("@NormalTrick_Create(アタックA, 110, 3)");
		bfsCmdList.Add("@NormalTrick_Create(アタックB, 8, 2)");
		
		//画像読み込み
			bfsCmdList.Add("@Load_Pic(tenyu_data/pic/sys/battle/cursor.png, MENU_CURSOR)");
			bfsCmdList.Add("@Load_Pic(tenyu_data/pic/sys/battle/timebar1.png, TIME_BAR1)");
			bfsCmdList.Add("@Load_Pic(tenyu_data/pic/sys/battle/timebar2.png, TIME_BAR2)");
			bfsCmdList.Add("@Load_Pic(tenyu_data/pic/sys/battle/hpbar.png, HP_BAR)");
			
			bfsCmdList.Add("@Load_Pic(tenyu_data/pic/battle/enemy01.bmp, pic_enemy01)");
			bfsCmdList.Add("@Load_Pic(tenyu_data/pic/battle/enemy02.bmp, pic_enemy02)");
			bfsCmdList.Add("@Load_Pic(tenyu_data/pic/battle/enemy03.bmp, pic_enemy03)");
			bfsCmdList.Add("@Load_Pic(tenyu_data/pic/battle/player.bmp, pic_player01)");
			bfsCmdList.Add("@Load_Pic(tenyu_data/pic/battle/player.bmp, pic_player02)");
			bfsCmdList.Add("@Load_Pic(tenyu_data/pic/battle/player.bmp, pic_player03)");
			
			bfsCmdList.Add("@Load_Pic(tenyu_data/pic/battle/background04.bmp, bg_01)");
			//Img_BattleBackGround = LoadGraph("tenyu_data/pic/sys/battle/background (4).bmp", true);		//いずれはコマンドから読み込む
		
		//PlayerSpeciesの生成（テスト用）
			bfsCmdList.Add("@Player_Create(プレイヤーA, 100, 5, 5, 1, pic_player01)");
			bfsCmdList.Add("@Player_Create(プレイヤーB, 100, 5, 5, 1, pic_player02)");
			bfsCmdList.Add("@Player_Create(プレイヤーC, 100, 5, 5, 1, pic_player03)");
			bfsCmdList.Add("@Player_Create(プレイヤーD, 100, 5, 5, 1, pic_player04)");
	
		//PlayerSpeciesの技リストセット（テスト用）
			bfsCmdList.Add("@PlayerTrick_Set(プレイヤーA, アタックA, アタックB)");
			bfsCmdList.Add("@PlayerTrick_Set(プレイヤーB, アタックA, アタックB)");
			bfsCmdList.Add("@PlayerTrick_Set(プレイヤーC, アタックA, アタックB)");
			bfsCmdList.Add("@PlayerTrick_Set(プレイヤーD, アタックA, アタックB)");

		//EnemySpeciesの生成（テスト用）
			bfsCmdList.Add("@Enemy_Create(エネミーA, 100, 5, 5, 1 ,pic_enemy01)");
			bfsCmdList.Add("@Enemy_Create(エネミーB, 100, 5, 5, 1 ,pic_enemy02)");
			bfsCmdList.Add("@Enemy_Create(エネミーC, 100, 5, 5, 1 ,pic_enemy03)");
			bfsCmdList.Add("@Enemy_Create(エネミーD, 100, 5, 5, 1 ,pic_enemy04)");
	
		//EnemySpeciesの技リストセット（テスト用）
			bfsCmdList.Add("@EnemyTrick_Set(エネミーA, アタックA, アタックB)");
			bfsCmdList.Add("@EnemyTrick_Set(エネミーB, アタックA, アタックB)");
			bfsCmdList.Add("@EnemyTrick_Set(エネミーC, アタックA, アタックB)");
			bfsCmdList.Add("@EnemyTrick_Set(エネミーD, アタックA, アタックB)");
		
		bfsCmdManager.Main(&bfsCmdList, &BImgBank, &PlayerSpeciesManager, &EnemySpeciesManager, &TrickManager);
	/////////////////////////////////////////////////////


	//メインのテキストボックスとオーバーラップ用テキストボックスの初期化
		TextBox1.Init(60, 370, WINDOW_WIDTH-80*2, 100, 3, 25*2, 16, WHITE, BLACK, 3);	//コンストラクタに書いたら起動しなくなった
		TextWrap1.Init(100, 100, 400, 300, 30, 30*2, 14, WHITE, GRAY, 3);  
		TextBox = &TextBox1;
		

	//ターゲット選択マーカー初期化
		TargetMarker.Init(LoadGraph("tenyu_data/pic/sys/battle/target.png", true));
	
	//仮///////
		PlayerSpeciesManager.SetBattleMember(3);


	SetTransColor(0, 0, 0);	//透過色指定
}

void CBattle::SetBackGround(const char* _pickey){
	Img_BattleBackGround = BImgBank.GetImg(_pickey);
}

void CBattle::SetPlayer(){
	PLAYER_NUM = PlayerSpeciesManager.BattleMember.size();
	if (PLAYER_NUM==0){
		ErrorDx("Error->Battle::SetPlayer->PLAYER_NUM=0.  SetPartyBattleMember!", __FILE__, __LINE__);
		return;
	}

	Player = new CPlayer[PLAYER_NUM];
	for (int i=0; i<PLAYER_NUM; i++){
		Player[i] = CPlayer(PlayerSpeciesManager.GetSpecies(PlayerSpeciesManager.BattleMember[i].c_str()));
	}
}
void CBattle::SetPlayer(const int _playerNum, ...){
	PLAYER_NUM=0;
	va_list args;
	va_start( args, _playerNum);	//targetが大きすぎたときの処置方法はないのか？
	
	if (_playerNum<=0){
		ErrorDx("Error->arg[playerNum] should >=1: playerNum=%d", __FILE__, __LINE__, _playerNum);
	}else{		
		PLAYER_NUM = _playerNum;
		Player = new CPlayer[PLAYER_NUM];
		for (int i=0; i<PLAYER_NUM; i++){
			Player[i] = CPlayer(PlayerSpeciesManager.GetSpecies(va_arg(args, char*)));		//target=1の時、一個目を返す（Not target=0）
		}
		va_end(args);
	}
}

void CBattle::SetEnemy(const int _enemyNum, ...){
	va_list args;
	va_start( args, _enemyNum);	//targetが大きすぎたときの処置方法はないのか？
	
	if (_enemyNum<=0){
		ErrorDx("Error->arg[enemyNum] should >=1: enemyNum=%d", __FILE__, __LINE__, _enemyNum);
	}else{
		std::vector<std::string> enemyList;
		for (int i=0; i<_enemyNum; i++){
			enemyList.push_back(va_arg(args, char*));		//target=1の時、一個目を返す（Not target=0）
		}
		va_end(args);
	}
}
void CBattle::SetEnemy(std::vector<std::string> _enemyList){
	if (_enemyList.size()==0) ErrorDx("Error->Battle::SetEnemy->enemyNum=0 :%d", __FILE__, __LINE__, _enemyList.size());

	ENEMY_NUM = _enemyList.size();
	Enemy = new CEnemy[ENEMY_NUM];
	for (int i=0; i<ENEMY_NUM; i++){
		Enemy[i] = CEnemy(EnemySpeciesManager.GetSpecies(_enemyList[i].c_str()));		//target=1の時、一個目を返す（Not target=0）
	}
}


void CBattle::BattleStart(int* _result, CFlagSet* _flagset_p, CField* _field_p, CMap* _map_p, CEveManager* _evemanager_p){
	//開始処理///////////////////////////////////////////////////
	
		//背景設定
			//SetBackGround("bg_01");

		//Player生成
			//SetPlayer(3, "プレイヤーA", "プレイヤーB", "プレイヤーC");

		//Enemy生成
			//SetEnemy(3, "エネミーA", "エネミーB", "エネミーC");

		//ActorへのﾅﾝﾊﾞﾘﾝｸﾞとTextBoxへの紐付け
			//Actorはnewでバトルごとに生成
			ACTOR_NUM = PLAYER_NUM + ENEMY_NUM;
			Actor = new CActor*[ACTOR_NUM];
			for (int i=0; i<ACTOR_NUM; i++){
				Actor[i] = ((i<MAX_PLAYER)? (CActor*)&Player[i]: (CActor*)&Enemy[i-MAX_PLAYER]);
				Actor[i]->FirstSet(i, &TextBox, &B_CmdList);
				Actor[i]->SetSystemImg(&BImgBank);
			}

		//初期値設定
			for (int i=0; i<PLAYER_NUM; i++){	//$雑なテスト用初期値設定
				Player[i].CreateBattleMenu();//※必ずAddTrickの後にすること（内部でBattleMenuを作っているため）
				Player[i].SetRect(WINDOW_WIDTH/4*(i+1), WINDOW_HEIGHT-200);
			}
			
			CEnemyPlanManager::GetInstance()->SetActor_p(Actor);	//EnemyPlanManagerはすべてのActorへのポインタを握る
			CEnemyPlanManager::GetInstance()->Init();
			for (int i=0; i<ENEMY_NUM; i++){
				Enemy[i].SetEnemyPlanManager(CEnemyPlanManager::GetInstance());
				Enemy[i].MakePlan();

				Enemy[i].SetRect(WINDOW_WIDTH/4*(i+1), 70);
			}


		EveManager_p = _evemanager_p;
		FlagSet_p = _flagset_p;
		B_CmdManager.Init(_map_p);


		TextBox->AddStock("戦闘開始！");
		TextBox->NextPage(&B_CmdList, FlagSet_p);


	//メインループ処理///////////////////////////////////////////////////
		int tmp_result;
		tmp_result = MainLoop();

	//終了処理///////////////////////////////////////////////////
		BattleFinish();
		*_result = tmp_result;
}

int CBattle::MainLoop(){	//戦闘中はこのループ内から出ない

	DebugDx("MainLoopStart");
	
	while(BasicLoop()){
		
		if( !TextBox->Main(&B_CmdList, FlagSet_p)){	//テキスト表示中はキー操作無効（テキスト送りはTextBox.Mainで判定）		

			if (ActionQueue.empty()){	//行動待機リストが空かチェック
			
				//$/////////////////////////////////////////////////////////
					if (CheckHitKeyDown(KEY_INPUT_W)) return WIN;
					if (CheckHitKeyDown(KEY_INPUT_L)) return LOSE;
				////////////////////////////////////////////////////////////

				for (int i=0; i<ACTOR_NUM; i++){
					if (Actor[i]->Main()) ActionQueue.push(Actor[i]);		//TimeBar進ませて必要なら行動待機リストに入れる
				}

			}else{
				if (ActionQueue.front()->Do()){
					ActionQueue.pop();
				}else{
					//行動待機リスト戦闘のキャラの行動が完了しなかったときにはなにもしない
				}
			}
		}

		////TextBoxなどによってCmdListに蓄積されたコマンドを処理////////////////////////////////////////
			B_CmdManager.Main(&B_CmdList, this, TextBox);

		////描画////////////////////////////////////////
			Draw();
	}
	return -1;
}

void CBattle::BattleFinish(){
	
	delete [] Actor;
	delete [] Player;
	delete [] Enemy;

	ACTOR_NUM = 0;
	ENEMY_NUM = 0;
	PLAYER_NUM = 0;

}

void CBattle::Draw(bool _screenflip, bool _textshowingstop, int dx, int dy, bool _playeralsoshake){
	
	if (ImgBackGround!=NULL){	//背景一枚絵モードのとき
		CVector picsize = GetGraphSize(ImgBackGround);
		DrawGraph(WINDOW_WIDTH/2-picsize.x/2, WINDOW_HEIGHT/2-picsize.y/2, ImgBackGround, true);
	}else{

		//背景描画////////////////////////////////////////////////////////////
		SetDrawBright(200, 200, 200);
		DrawExtendGraph(0,0,WINDOW_WIDTH, WINDOW_HEIGHT, Img_BattleBackGround,true);
		SetDrawBright(255,255,255);
		/////////////////////////////////////////////////////////////////////

		//PlayerとEnemyの描画///////////////////////////////////////////////////////////////
		for (int i=0; i<MAX_PLAYER; i++){
			Player[i].Draw(dx, dy);
		}
		for (int i=0; i<MAX_ENEMY; i++){
			Enemy[i].Draw(dx, dy);
		}

		//ターゲットマーカーの描画//////////////////////////////////////////////
		TargetMarker.Draw(dx,dy);

	}

	////////////////////////////////////////////////////////////////////////////////////////
	//テキストボックス描画//////////////////////////////////////////////////////////////////
	TextBox->Draw(_textshowingstop);
	////////////////////////////////////////////////////////////////////////////////////////
	

	///テスト用//////////////////////////////////////////////////////////////////////////
	SetDrawBlendMode(DX_BLENDMODE_ALPHA , 150+60*sin(2*PI*GetNowCount()/((double)2000)));
	DrawCenterString(WINDOW_WIDTH/2,WINDOW_HEIGHT/2,"戦闘中---WまたはLで終了",RED);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
	///////////////////////////////////////////////////////////////////////////////////////

	if (_screenflip)	{BasicLoop();}
}

void CBattle::ChangeTextMode(bool _box, const char* _eventtext){
	if (_box){
		TextBox = &TextBox1;
	}else{
		TextBox = &TextWrap1;

		if (_eventtext!=NULL){	//EveManager::CopyOriginalEventを汎用性を上げて改善。これでTextWrap1に@EventWrapの内容を渡せた
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


