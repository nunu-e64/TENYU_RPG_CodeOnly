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


bool CBattle::Init(){	//Field.Init()で呼び出す	//14/06/26
	PlayerSpeciesManager.Clear();
	EnemySpeciesManager.Clear();
	BImgBank.Clear();
	TrickManager.Clear();
	ACTOR_NUM = 0;
	PLAYER_NUM = 0;
	ENEMY_NUM = 0;
	
	SetTransColor(255, 0, 255);	//透過色指定	//最終的には統一しないとな…$
	
	//Load.cppを通して.rpgの読み込み/////////////////////
	
		//外部テキストのロード
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

	//メインのテキストボックスとオーバーラップ用テキストボックスの初期化
		TextBox1.Init(60, 370, WINDOW_WIDTH-80*2, 100, 3, 25*2, 16, WHITE, BLACK, 3);	//コンストラクタに書いたら起動しなくなった
		TextWrap1.Init(100, 100, 400, 300, 30, 30*2, 14, WHITE, GRAY, 3);  
		TextBox = &TextBox1;
	
	//どこに書くべきか･･･Field?///////
		PlayerSpeciesManager.SetMemberList();
	//////////////////

	SetTransColor(0, 0, 0);	//透過色指定
	return true;
}
void CBattle::Term(){	//タイトルに戻るときに~CField()から呼び出し

}
void CBattle::BattleReady(CFlagSet* _flagset_p, CMap* _map_p, CEveManager* _evemanager_p){
	//開始処理///////////////////////////////////////////////////	
		
		//ActorへのﾅﾝﾊﾞﾘﾝｸﾞとTextBoxへの紐付け
			ACTOR_NUM = PLAYER_NUM + ENEMY_NUM;
			Actor = new CActor*[ACTOR_NUM];
			for (int i=0; i<ACTOR_NUM; i++){
				Actor[i] = ((i<PLAYER_NUM)? (CActor*)&Player[i]: (CActor*)&Enemy[i-PLAYER_NUM]);
				Actor[i]->FirstSet(PLAYER_NUM, ENEMY_NUM, i, &TextBox, &B_CmdList);
				Actor[i]->SetSystemImg(&BImgBank);
			}

		//初期値設定
			for (int i=0; i<PLAYER_NUM; i++){
				Player[i].CreateBattleMenu();//※必ずAddTrickの後にすること（内部でBattleMenuを作っているため）
				Player[i].SetRect(WINDOW_WIDTH/4*(i+1), WINDOW_HEIGHT-200);
			}
			
			CEnemyPlanManager::GetInstance()->SetActor_p(Actor);	//EnemyPlanManagerはすべてのActorへのポインタを握る
			CEnemyPlanManager::GetInstance()->Init(PLAYER_NUM, ENEMY_NUM);
			for (int i=0; i<ENEMY_NUM; i++){
				Enemy[i].SetEnemyPlanManager(CEnemyPlanManager::GetInstance());
				Enemy[i].MakePlan();

				Enemy[i].SetRect(WINDOW_WIDTH/4*(i+1), 70);
			}

		//ターゲット選択マーカー初期化
			SetTransColor(255, 0, 255);	//透過色指定
			TargetMarker.Init(ACTOR_NUM, PLAYER_NUM, ENEMY_NUM, LoadGraph("tenyu_data/pic/sys/battle/target.png", true));
			SetTransColor(0, 0, 0);	//透過色指定

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
		//メインループ処理///////////////////////////////////////////////////
			int tmp_result = MainLoop();

		//終了処理///////////////////////////////////////////////////
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

		for (int i =0; i<tmpparty.size(); i++){		
			DebugDx("enemy:%s", tmpparty[i]->GetName().c_str());
		}

		SetEnemy(tmpparty);
		SetBackGround("bg_01");	//これもMapNumとChipNum依存でBattleに持たせておく$
		DebugDx("Encount_fin");
		return true;
	}else{
		return false;
	}
}

void CBattle::SetBackGround(const char* _pickey){
	Img_BattleBackGround = BImgBank.GetImg(_pickey);
}

void CBattle::SetPlayer(){	//隊列に基づいて選出
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
void CBattle::SetPlayer(const int _playerNum, ...){	//パーティ自由指定用（イベント戦闘）
	PLAYER_NUM=0;
	va_list args;
	va_start( args, _playerNum);	//targetが大きすぎたときの処置方法はないのか？
	
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

void CBattle::SetEnemy(const int _enemyNum, ...){	//出現モンスター自由指定用
	va_list args;
	va_start( args, _enemyNum);	//targetが大きすぎたときの処置方法はないのか？
	
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


int CBattle::MainLoop(){	//戦闘中はこのループ内から出ない
	int result;
	
	do{		
		if( !TextBox->Main(&B_CmdList, FlagSet_p)){	//テキスト表示中はキー操作無効（テキスト送りはTextBox.Mainで判定）		

			if (ActionQueue.empty()){	//行動待機リストが空かチェック
			
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

			if ((result=ResultCheck())!=-1){
				B_CmdManager.Main(&B_CmdList, this, TextBox);
				Draw();
				return result;
			}

		}

		////TextBoxなどによってCmdListに蓄積されたコマンドを処理////////////////////////////////////////
			B_CmdManager.Main(&B_CmdList, this, TextBox);

		////描画////////////////////////////////////////
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
		//事前登録したコマンドをフィールドのコマンドリストに挿入
			if (strlen(WinCommand)) _fieldcmdlist->Add(WinCommand);

		//金と経験値計算
			{int gold = BattleCalc::CalcGold(1, 2);
			int exp = BattleCalc::CalcExp(1, 2);
			PlayerSpeciesManager.AddGold(gold);
			//PlayerSpeciesManager.AddExp(exp);
			char text[256];
			sprintf_s(text, "取得Gold：%d", gold); TextBox->AddStock(text);
			sprintf_s(text, "取得Exp：%d", exp);	TextBox->AddStock(text);
			}

		//Player元データに保存
			PlayerSpeciesManager.CopyValue(PLAYER_NUM, Player);	//PlayerSpecies配列で渡したいがキャストではメモリ配置の関係上配列での参照がずれるため断念

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

	Draw();	//裏画面に描画（画面切り替え演出用）

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
		for (int i=0; i<ACTOR_NUM; i++){
			Actor[i]->Draw(dx, dy);
		}

		//ターゲットマーカーの描画//////////////////////////////////////////////
		SetDrawBright(255,255,255);
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


//////////////////////////////////////////////////////////////
void CBattle::CTargetMarker::Init(int _actornum, int _playernum, int _enemynum, int _img){
	ACTOR_NUM = _actornum;
	PLAYER_NUM = _playernum;
	ENEMY_NUM = _enemynum;
	Img = _img;
	Visible = false;
	EnemySide = true;
	Index = 0;
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
		SetVisible(false);
	}

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
