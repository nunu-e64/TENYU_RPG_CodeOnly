#include "../Define.h"
#include "Battle.h"


CBattle::CBattle(){

	TextBox = &TextBox1;
	Img_BattleBackGround = NULL;
	
}

void CBattle::Init(){	//Field.Init()で呼び出す	//14/06/26
	//Field.Initにいずれ移動すると思われる
		TrickManager.Clear();
		trick_tag newtrick;{
			strcpy_s(newtrick.Name, "アタックマジックA");
			newtrick.Cost = 3;
			newtrick.Power = 110;
			newtrick.TargetType = newtrick.SINGLE;
			TrickManager.Add(newtrick);
		}
		{	strcpy_s(newtrick.Name, "アタックマジックB");
			newtrick.Cost = 2;
			newtrick.Power = 8;
			newtrick.TargetType = newtrick.SINGLE;
			TrickManager.Add(newtrick);
		}

	//メインのテキストボックスとオーバーラップ用テキストボックスの初期化
		TextBox1.Init(60, 370, WINDOW_WIDTH-80*2, 100, 3, 25*2, 16, WHITE, BLACK, 3);	//コンストラクタに書いたら起動しなくなった
		TextWrap1.Init(100, 100, 400, 300, 30, 30*2, 14, WHITE, GRAY, 3);  
		TextBox = &TextBox1;
		
	SetTransColor(255, 0, 255);	//透過色指定	//最終的には統一しないとな…$

	//画像読み込み
		ImgBank.clear();
		PlayerImgBank.clear();
		EnemyImgBank.clear();

		Img_BattleBackGround = LoadGraph("tenyu_data/pic/sys/battle/background (4).bmp", true);		//いずれはコマンドから読み込む
		ImgBank.insert( std::map<int, int>::value_type( MENU_CURSOR, LoadGraph("tenyu_data/pic/sys/battle/cursor.png")) );
		ImgBank.insert( std::map<int, int>::value_type( TIME_BAR1, LoadGraph("tenyu_data/pic/sys/battle/timebar1.png")) );
		ImgBank.insert( std::map<int, int>::value_type( TIME_BAR2, LoadGraph("tenyu_data/pic/sys/battle/timebar2.png")) );
		ImgBank.insert( std::map<int, int>::value_type( HP_BAR, LoadGraph("tenyu_data/pic/sys/battle/hpbar.png")) );

		EnemyImgBank.push_back(LoadGraph("tenyu_data/pic/sys/battle/enemy01.bmp"));
		EnemyImgBank.push_back(LoadGraph("tenyu_data/pic/sys/battle/enemy02.bmp"));
		EnemyImgBank.push_back(LoadGraph("tenyu_data/pic/sys/battle/enemy03.bmp"));
		
		PlayerImgBank.push_back(LoadGraph("tenyu_data/pic/sys/battle/player.bmp"));
		PlayerImgBank.push_back(LoadGraph("tenyu_data/pic/sys/battle/player.bmp"));
		PlayerImgBank.push_back(LoadGraph("tenyu_data/pic/sys/battle/player.bmp"));


	//ターゲット選択マーカー初期化
		TargetMarker.Init(LoadGraph("tenyu_data/pic/sys/battle/target.png", true));
	
	SetTransColor(0, 0, 0);	//透過色指定

	
	//ActorへのﾅﾝﾊﾞﾘﾝｸﾞとTextBoxへの紐付け
	//Actorはnewでバトルごとに生成するようにしてもいい
		int i;
		for (i=0; i<ACTOR_NUM; i++){
			Actor[i] = ((i<MAX_PLAYER)? (CActor*)&Player[i]: (CActor*)&Enemy[i-MAX_PLAYER]);
			Actor[i]->FirstSet(i, &TextBox, &B_CmdList, &ImgBank);
			Actor[i]->SetImg(((i<MAX_PLAYER)? PlayerImgBank[i]: EnemyImgBank[i-MAX_PLAYER]));
		}

}

void CBattle::Battle(int* _result, CFlagSet* _flagset_p, CField* _field_p, CMap* _map_p, CEveManager* _evemanager_p){
	//開始処理///////////////////////////////////////////////////
		for (int i=0; i<ACTOR_NUM; i++){	//$ざっつ
			Actor[i]->ClearTrick();
			Actor[i]->AddTrick(TrickManager.GetTrick("アタックマジックA"));
			Actor[i]->AddTrick(TrickManager.GetTrick("アタックマジックB"));
			Actor[i]->SetValue(5, 5, 1, 100);	//※必ずAddTrickの後にすること（内部でBattleMenuを作っているため）
		}

		for (int i=0; i<MAX_ENEMY; i++){
			Enemy[i].SetRect(WINDOW_WIDTH/4*(i+1), 70);
		}
		for (int i=0; i<MAX_PLAYER; i++){
			Player[i].SetRect(WINDOW_WIDTH/4*(i+1), WINDOW_HEIGHT-200);
		}
			

		EveManager_p = _evemanager_p;
		FlagSet_p = _flagset_p;
		B_CmdManager.Init(_map_p);


		TextBox->AddStock("戦闘開始！");/*
		TextBox->AddStock("@EventWrap(wrap_sample, [戦闘])");
		TextBox->AddStock("@Anten(1000)");
		TextBox->AddStock("@Meiten(2000)");
		TextBox->AddStock("@Window_Shake(30,5)");*/
		TextBox->NextPage(&B_CmdList, FlagSet_p);


	//メインループ処理///////////////////////////////////////////////////
		int tmp_result;
		tmp_result = MainLoop();

	//終了処理///////////////////////////////////////////////////
		*_result = tmp_result;
}

int CBattle::MainLoop(){	//戦闘中はこのループ内から出ない
	
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
		DrawCenterString(tmpRect.Center().x, tmpRect.Top-10*sin(timecount*(PI/2)/20), WHITE, "%d", damage); 
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
	int actorindex = between(0, ACTOR_NUM-1, _actorindex); 

	if (!_deadok && _battle->Actor[actorindex]->GetHp()==0){
		return;
	}else{
		_battle->Actor[actorindex]->SetTarget(Index + (EnemySide?MAX_PLAYER:0));
		SetVisible(false);
	}

}

