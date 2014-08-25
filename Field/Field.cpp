#include "../Define.h"
#include "Field.h"
#include "Load.h"
#include "../Battle/Battle.h"

static CBattle Battle;

///メンバ変数にするとバグる////////////////
static CTextBox TextBox1;
static CTextWrap TextWrap1;
///////////////////////////////////////////


CField::~CField(){
	Map.Init();
	EveManager.Init();
}

bool CField::Init(playdata_tag* _playdata_p, const int _dnum){
	CHECK_TIME_START

		//諸変数の初期化
			NowMap=0;
			X=0; Y=0; OldX=X; OldY=Y;
			Dir=DOWN; 
			Step=0; Dx=0; Dy=0; 
			Visible=true;
			ImgBackGround = NULL;
			TextAutoPlaySpeed = 4;

		//メインのテキストボックスとオーバーラップ用テキストボックスの初期化
			TextBox1.Init(60, 370, WINDOW_WIDTH-80*2, 100, 3, 25*2, 16, WHITE, BLACK, TextAutoPlaySpeed);
			TextWrap1.Init(100, 100, 400, 300, 30, 30*2, 14, WHITE, GRAY, TextAutoPlaySpeed);  
			TextBox = &TextBox1;

		////WorldManagerのポインタ変数に代入
		//	EveManager_p = &EveManager;
		//	FlagSet_p = &FlagSet;
		//	CmdList_p = &CmdList;
		//	DebugDx("OK");
		//	

		//CBattleの初期化
			Battle.Init();
			
		//外部テキストのロード
			CLoad SystemLoad;
			CLoad ScenarioLoad;
			CFirstSetCmdManager FirstSetCmdManager;
			CCmdList SystemCmdList;
			
			if ((SystemLoad.LoadAddText("tenyu_data/system.rpg"))
				&&(ScenarioLoad.LoadAddText("tenyu_data/scenario.rpg"))
				&&(ScenarioLoad.LoadAddText("tenyu_data/event.rpg"))){
			
				SystemLoad.CommandCopy(&SystemCmdList);
				FirstSetCmdManager.Main(&SystemCmdList, this, &Map, &EveManager);

				ScenarioLoad.EventTextCopy(&EveManager);	//順序に注意（CmdManager.Mainの後）	
			
			}else{
				return false;
			}
			
		//セーブデータの読み込み
			PlayData_p = _playdata_p;
			if (!StartSet(_dnum)) return false;

	CHECK_TIME_END("Init_Field")
		
	return true;
}

int CField::MainLoop(){	//ゲーム中はこのループ内から出ない


	while( BasicLoop() ){
	
		CHECK_TIME_START	

		if( !TextBox->Main(&CmdList, &FlagSet)){	//テキスト表示中はキー操作無効（テキスト送りはTextBox.Mainで判定）
			
			if (OldX!=X||OldY!=Y) {		//アクションコマンドによる移動判定用
				OldX=X; OldY=Y;
				if (CheckEvent(true)) {
					TextBox->NextPage(&CmdList, &FlagSet);		//足元にテキストが設定してあれば表示
				}

			}else if (CheckHitKeyDown(KEY_INPUT_OK)){
				if (CheckEvent(false)) TextBox->NextPage(&CmdList, &FlagSet);		//目の前のオブジェクトにテキストが設定してあれば表示
				
			}else{
				//歩行/////////////////////////////////////////////////////
				int walkspeed = ((CheckHitKey(KEY_INPUT_LSHIFT)||CheckHitKey(KEY_INPUT_RSHIFT))? 4:2);
				#ifdef DEBUG_MODE
					walkspeed = ((CheckHitKey(KEY_INPUT_LCONTROL)||CheckHitKey(KEY_INPUT_RCONTROL))? MAP_CHIP_SIZE:walkspeed);
				#endif
				
				//PUSHBLOCKを押したときだけWalkからtrueが返る
				if(CheckHitKey(KEY_INPUT_RIGHT)){		if(Walk(RIGHT, walkspeed))TextBox->NextPage(&CmdList, &FlagSet);
				}else if(CheckHitKey(KEY_INPUT_LEFT)){	if(Walk(LEFT, walkspeed))TextBox->NextPage(&CmdList, &FlagSet);
				}else if(CheckHitKey(KEY_INPUT_DOWN)){	if(Walk(DOWN, walkspeed))TextBox->NextPage(&CmdList, &FlagSet);
				}else if(CheckHitKey(KEY_INPUT_UP)){	if(Walk(UP, walkspeed))TextBox->NextPage(&CmdList, &FlagSet);
				}
				X = between(0, MAP_SIZE-1, (int)X);
				Y = between(0, MAP_SIZE-1, (int)Y);
				
				if (OldX!=X||OldY!=Y) {		//移動しなかった時は足元チェックしない
					OldX=X; OldY=Y;
					if (CheckEvent(true)) TextBox->NextPage(&CmdList, &FlagSet);		//足元にテキストが設定してあれば表示
				}
				///////////////////////////////////////////////////////////
				
			}

			////テスト用：ゲームオーバー＆クリア＆セーブ///////////$
				if (CheckHitKey(KEY_INPUT_S) && CheckHitKey(KEY_INPUT_LCONTROL)){
					SaveData(0);
					TextBox->AddStock("0番に上書き保存しました");	TextBox->NextPage(&CmdList, &FlagSet);

				}else if (CheckHitKey(KEY_INPUT_LCONTROL)){
					int tmpdnum=-1;
					if (CheckHitKey(KEY_INPUT_0)){			tmpdnum = 0;
					}else if (CheckHitKey(KEY_INPUT_1)){	tmpdnum = 1;
					}else if (CheckHitKey(KEY_INPUT_2)){	tmpdnum = 2;
					}else if (CheckHitKey(KEY_INPUT_3)){	tmpdnum = 3;						
					}
					if (tmpdnum!=-1){
						//セーブデータ名の自由入力
						char dataname[32];	dataname[0]='\0';
						char inputchar;		char inputmessage[64];
						ClearInputCharBuf() ;
						while(BasicLoop()){
							// 文字入力バッファから文字を取得する
							inputchar = GetInputChar( TRUE ) ;
							// 操作コード以外の文字入力があった場合のみ処理を行う
							if( inputchar == CTRL_CODE_CR && strlen(dataname)>0){		//Enter
								break;
							}else if( inputchar == CTRL_CODE_BS && strlen(dataname)>0){	//BackSpace
								dataname[strlen(dataname)-1]='\0';
							}else{
								switch(inputchar){
								case '.':	case '|':	case '\\':	case '/':	case ':':	case '>':	case '<':	case '?':	case '*':	case '"':	case ' ':	case '%':	//なぜか知らんが%を使用すると応答なしエラーになる	//フォルダに使用できない文字その他の排除
									break;
								default:
									if( inputchar != 0 && inputchar >= CTRL_CODE_CMP && strlen(dataname)<ARRAY_SIZE(dataname)-1){
										dataname[strlen(dataname)+1]='\0';
										dataname[strlen(dataname)]=inputchar;
									}
								}
							}

							sprintf_s(inputmessage, "Input Save Data Name->%s%s", dataname, (strlen(dataname)<ARRAY_SIZE(dataname)-1?"_":""));
							DrawString(0, 0, inputmessage, WHITE);
						}
						if (SaveData(tmpdnum, dataname)){
							char tmpmessage[32];			sprintf_s(tmpmessage, "%d番にセーブしました", tmpdnum);
							TextBox->AddStock(tmpmessage);	TextBox->NextPage(&CmdList, &FlagSet);
						}
					}

				}else if (CheckHitKey(KEY_INPUT_1)){
					return MODE_GAMECLEAR;
				}else if (CheckHitKey(KEY_INPUT_2)){
					return MODE_GAMEOVER;
				}else if (CheckHitKeyDown(KEY_INPUT_ESCAPE) || CheckHitKey(KEY_INPUT_3)){
					return MODE_BACKTOTITLE;
				}else if (CheckHitKeyDown(KEY_INPUT_P)){
					CmdList.Add("@AutoPlay_Set(true)");
				}else if (CheckHitKeyDown(KEY_INPUT_B)){;
					int result;
					Battle.Battle(&result, &FlagSet, this, &Map, &EveManager);		//コマンド化を
					if (result==WIN) TextBox->AddStock("勝利");
					if (result==LOSE) TextBox->AddStock("敗北");
					TextBox->NextPage(&CmdList, &FlagSet);
				}


			////////////////////////////////////////////////
		}

		CHECK_TIME_END("Main_Walk")	

		////デバッグの時にはプレイヤー座標をタイトルバーに表示////////////////////////////////////////
			#ifdef DEBUG_MODE
				SetTitle("Pos_%d:%d Data_%d:%d", X, Y, Map.GetMapData(NowMap, X, Y, 0),Map.GetMapData(NowMap, X, Y, 1));
			#endif

		////TextBoxなどによってCmdListに蓄積されたコマンドを処理////////////////////////////////////////
			CHECK_TIME_START	FieldCmdManager.Main(&CmdList, this, &Map, TextBox, &EveManager);	CHECK_TIME_END("Command.Main")

		////描画////////////////////////////////////////
			CHECK_TIME_START
			Draw();
			CHECK_TIME_END("Draw")

				
	}
	return MODE_GAMEEND;
}

void CField::Draw(bool _screenflip, bool _textshowingstop, int dx, int dy, bool _playeralsoshake){
	
	if (ImgBackGround!=NULL){	//背景一枚絵モードのとき
		CVector picsize = GetGraphSize(ImgBackGround);
		DrawGraph(WINDOW_WIDTH/2-picsize.x/2, WINDOW_HEIGHT/2-picsize.y/2, ImgBackGround, true);
	}else{
		//マップ描画////////////////////////////////////////////////////////////////////////////
		CHECK_TIME_START2	Map.Draw(NowMap, X, Y, dx, dy);			CHECK_TIME_END2("Map.Draw")
		CHECK_TIME_START2	EveManager.Draw(NowMap, X, Y, false, dx, dy);	CHECK_TIME_END2("EveManager.Draw_under")
			//プレイヤー
			if(!_playeralsoshake){
				if(Visible)Map.DrawPlayer(ImgPlayer[Dir*4+mod(Step,4)], Dx, Dy);
			}else{
				if(Visible)Map.DrawPlayer(ImgPlayer[Dir*4+mod(Step,4)], Dx-dx, Dy-dy);
			}
	
		CHECK_TIME_START2	EveManager.Draw(NowMap, X, Y, true, dx, dy);	CHECK_TIME_END2("EveManager.Draw_over")
	}

	////////////////////////////////////////////////////////////////////////////////////////
	//テキストボックス描画//////////////////////////////////////////////////////////////////
	TextBox->Draw(_textshowingstop);
	////////////////////////////////////////////////////////////////////////////////////////
	
	if (_screenflip)	{BasicLoop();}
}

	
bool CField::Walk(int _dir, int _walkspeed, bool _eventwalk, bool _walk, int _fade){	
	if(_walk){
		SetMyDir(_dir);
		if(_walkspeed<0) _dir=sys::TurnDir(_dir, 2);
		if (CheckHitKey(KEY_INPUT_A) && !_eventwalk)return false;	//Aを押しながらで方向転換のみ
	}

	if (Map.GetMapData(NowMap, (X+((_dir==RIGHT)? 1: ((_dir==LEFT)? -1: 0)))%MAP_SIZE, (Y+((_dir==DOWN)? 1: ((_dir==UP)? -1: 0)))%MAP_SIZE, 1)) return false;	//障害物の有無を確認
	if (!_eventwalk) if (CheckEvent(false, true)) return true;	//押せるブロック（PUSH_BLOCK）のイベント有無をチェック		//_eventwalkの時は通らないようにした方がいいかも
	if (!EveManager.CheckWalkable(NowMap, (X+((_dir==RIGHT)? 1: ((_dir==LEFT)? -1: 0)))%MAP_SIZE, (Y+((_dir==DOWN)? 1: ((_dir==UP)? -1: 0)))%MAP_SIZE)) return false;	//NPCorBLOCKの有無を確認

	int d=0, oldd=0;	//delta;
	int dx=0, dy=0;
	if (_walkspeed==0) _walkspeed=2;
	if (_walkspeed<0) _walkspeed=-_walkspeed;

	if(_walk)(++Step)%=4;
	while(d!=MAP_CHIP_SIZE){
		oldd=d;
		d = between(-MAP_CHIP_SIZE, MAP_CHIP_SIZE, d+_walkspeed);
		if (oldd/(MAP_CHIP_SIZE/2)<1 && d/(MAP_CHIP_SIZE/2)>=1) {
			if(_walk)(++Step)%=4;
		}
		dx = ((_dir==RIGHT)? d: ((_dir==LEFT)? -d: 0));
		dy = ((_dir==DOWN)? d: ((_dir==UP)? -d: 0));
		
		Draw(true, true, dx, dy);
	};

	switch(_dir){
	case RIGHT:
		X++;	break;
	case LEFT:
		X--;	break;
	case UP:
		Y--;	break;
	case DOWN:
		Y++;	break;
	}
		
	return false;
}

void CField::SetPosition(int _mapnum, int _x, int _y, bool _d){
	if (_mapnum>=0){
		MAP_MAX_CHECK(_mapnum,);	
		NowMap = _mapnum;
	}
	X = (_d? X+_x:_x) % MAP_SIZE;
	Y = (_d? Y+_y:_y) % MAP_SIZE;
}
void CField::SetMyPic(const int _img[CHARA_PIC_NUM], const char* _pickey){
	for(int i=0; i<CHARA_PIC_NUM; i++){
		ImgPlayer[i] = _img[i];
	}
	strcpy_s(PlayerPicKey, _pickey);
}
void CField::Jump(){
	Dy=-5;	
	for(int i=0; i<5; i++){
		Draw(true, true);
	}
	Dy=0;
	Draw(true, true);
}



////////private/////////////////////////////////////////////////////////////////////////////////////////////
bool CField::CheckEvent(bool _foot, bool _push){
	char** addtext = NULL;
	bool event_happened=false;
	int count;		//EveManagerからEveObjのCountを受け取るが変更はできない（Count加算はEveManager.GetTextで行われる）

	if (_push){
			if (EveManager.GetText(addtext, count, NowMap, (X+((Dir==RIGHT)?1:((Dir==LEFT)? -1:0)))%MAP_SIZE, (Y+((Dir==DOWN)? 1:((Dir==UP)?-1:0)))%MAP_SIZE, Dir, PUSHBLOCK)){
				event_happened = true;
			}
	}else if (!_foot){
		for (int k = WALKABLE_NUM+1; k < UNDERDRAW_NUM; k++){
			if (k==PUSHBLOCK) continue;
			if (EveManager.GetText(addtext, count, NowMap, (X+((Dir==RIGHT)?1:((Dir==LEFT)? -1:0)))%MAP_SIZE, (Y+((Dir==DOWN)? 1:((Dir==UP)?-1:0)))%MAP_SIZE, Dir, k)){
				event_happened = true;
				break;
			}
		}
	}else{
		for (int k = 0; k < WALKABLE_NUM; k++){
			if (EveManager.GetText(addtext, count, NowMap, X, Y, Dir, k)){
				event_happened = true;
				break;
			}
		}		
	}


	if (!event_happened) goto finish;	//イベント発生しなければメモリ解放だけして終了
	
	if (! TextBox->AddStock(addtext, Dir, count)) ErrorDx("Error->AddText", __FILE__, __LINE__);
	int i;
	for (i=0; !mystrcmp(addtext[i], EOP) ; i++){
		delete [] addtext[i];
		if (i > 10000) {ErrorDx("CriticalError->NotFound""EOP""", __FILE__, __LINE__); break;}
	}
	delete [] addtext[i];
	delete [] addtext;


finish:
	return event_happened;
}

bool CField::StartSet(const int _dnum){	//PlayDataに格納された読み込みセーブデータを各変数に振り分けて代入
	CCmdList PlayDataCmdList;
	char bufcmd[256];

	if (_dnum<-1 || _dnum>=PLAYDATA_NUM) {
		ErrorDx("GameStartError->dnum:%d", __FILE__, __LINE__, _dnum);
		return false;
	}

	if (_dnum!=-1 && PlayData_p[_dnum].Exist){
		
		sprintf_s(bufcmd, "@Position_Set(me, %d,%d,%d,%s)", PlayData_p[_dnum].NowMap, PlayData_p[_dnum].X, PlayData_p[_dnum].Y, PlayData_p[_dnum].PlayerPicKey);		PlayDataCmdList.Add(bufcmd);
		sprintf_s(bufcmd, "@Dir_Set(me,%d)", PlayData_p[_dnum].Dir);																						PlayDataCmdList.Add(bufcmd);
		FieldCmdManager.Main(&PlayDataCmdList, this, &Map, TextBox, &EveManager);
		
		for (unsigned int i = 0; i < PlayData_p[_dnum].EveObj.size(); i++){
			sprintf_s(bufcmd, "@Position_Set(%s,%d,%d)", PlayData_p[_dnum].EveObj[i].Name, PlayData_p[_dnum].EveObj[i].Dx/MAP_CHIP_SIZE, PlayData_p[_dnum].EveObj[i].Dy/MAP_CHIP_SIZE);	PlayDataCmdList.Add(bufcmd);
			if (PlayData_p[_dnum].EveObj[i].Kind==NPC){
				sprintf_s(bufcmd, "@Dir_Set(%s,%d)", PlayData_p[_dnum].EveObj[i].Name, PlayData_p[_dnum].EveObj[i].Dir);																PlayDataCmdList.Add(bufcmd);
			}
			sprintf_s(bufcmd, "@Count_Set(%s,%d)", PlayData_p[_dnum].EveObj[i].Name, PlayData_p[_dnum].EveObj[i].Count);																PlayDataCmdList.Add(bufcmd);
			sprintf_s(bufcmd, "@Pic_Set(%s,%s)", PlayData_p[_dnum].EveObj[i].Name, PlayData_p[_dnum].EveObj[i].PicKey);																PlayDataCmdList.Add(bufcmd);
			sprintf_s(bufcmd, "@Alpha_Set(%s,%d)", PlayData_p[_dnum].EveObj[i].Name, PlayData_p[_dnum].EveObj[i].Alpha*100/255);														PlayDataCmdList.Add(bufcmd);
			sprintf_s(bufcmd, "@Visible_Set(%s,%d)", PlayData_p[_dnum].EveObj[i].Name, PlayData_p[_dnum].EveObj[i].Visible);															PlayDataCmdList.Add(bufcmd);

			char tmp[256];
			sprintf_s(tmp, "%d",PlayData_p[_dnum].EveObj[i].EffectNumCmd[0]);
			for (int j=1; j<ARRAY_SIZE(PlayData_p[_dnum].EveObj[i].EffectNumCmd); j++){
				sprintf_s(tmp, "%s,%d", tmp, PlayData_p[_dnum].EveObj[i].EffectNumCmd[j]);
			}
			sprintf_s(bufcmd, "@Effect_Set(%s,%d,%s)", PlayData_p[_dnum].EveObj[i].Name, PlayData_p[_dnum].EveObj[i].Effect, tmp);													PlayDataCmdList.Add(bufcmd);

			FieldCmdManager.Main(&PlayDataCmdList, this, &Map, TextBox, &EveManager);
		}

		FlagSet = PlayData_p[_dnum].FlagSet;
		
		PlayData_p[_dnum].Exist = false;		//これの必要性よくわかんない14/05/07
	}
	
	if (!CheckHitKeyDown(KEY_INPUT_ESCAPE)){
		Map.CreateMapGraph(NowMap);
		return true;
	}else{
		return false;
	}
}

bool CField::SaveData(int _dnum, const char _dataname[32]){
	char filename[256];
	FILE *fp;
	
	//セーブデータ番号が適正かチェック
		if (_dnum<0 || _dnum>=PLAYDATA_NUM) {
			ErrorDx("Error->SaveDataNumber too small or big:%d", __FILE__, __LINE__, _dnum);
			return false;
		}
	
	//セーブフォルダをリネーム又は新規作成（_datanameが空文字列のときはセーブ番号だけを頼りに上書き保存する）
		char olddirname[256];
		char newdirname[256];
		if (strlen(_dataname) > 0){
			sprintf_s(olddirname, "tenyu_data/save/%s", PlayData_p[_dnum].DataName);
			sprintf_s(newdirname, "tenyu_data/save/%s", _dataname); 

			if (rename(olddirname, newdirname) != 0){
				if (_mkdir(newdirname) !=0 ){
					ErrorDx("SaveError->MakeDirectoryError:%s", __FILE__, __LINE__, newdirname);
					return false;
				}
			}
			strcpy_s(PlayData_p[_dnum].DataName, _dataname);
		}else{
			sprintf_s(newdirname, "tenyu_data/save/%s", PlayData_p[_dnum].DataName);
			_mkdir(newdirname);
		}
		
	//セーブデータ名一覧の保存（既存のものもすべて上書き）
		fopen_s(&fp, "tenyu_data/save/dataname.rpg", "w" );
		for (int i = 0; i < PLAYDATA_NUM; i++){
			fputs(PlayData_p[i].DataName, fp);		
			fputs("\n", fp);
		}
		fclose(fp);

		
	///セーブデータ番号に基づいてセーブファイルを開く/////////////////////////////////////////////////////////////////////////////////////
		for (int i=0; i<3; i++){
			switch(i){
			case 0:
				sprintf_s(filename, "tenyu_data/save/%s/pos.dat", PlayData_p[_dnum].DataName);
				break;
			case 1:
				sprintf_s(filename, "tenyu_data/save/%s/flg.dat", PlayData_p[_dnum].DataName);
				break;
			case 2:
				sprintf_s(filename, "tenyu_data/save/%s/eve.dat", PlayData_p[_dnum].DataName);
				break;
			default:
				//あとはアイテム、キャラステータス、金、と？ $
				break;
			}

			//ファイルを開く
			fopen_s(&fp, filename, "wb" );

			/////////////////////////////////////////////////////////////////////////////////
			switch(i){
			case 0:
				fwrite(&NowMap, sizeof(NowMap), 1, fp);
				fwrite(&X, sizeof(char), 1, fp);
				fwrite(&Y, sizeof(char), 1, fp);
				fwrite(&Dir, sizeof(char), 1, fp);
				fwrite(&PlayerPicKey, sizeof(char), sizeof(PlayerPicKey), fp);
				break;
			case 1:
				for(unsigned int i=0; i<FlagSet.Flag.size(); i++){
					fwrite(&FlagSet.Flag[i].Key, sizeof(char), sizeof(FlagSet.Flag[i].Key), fp);
					fwrite(&FlagSet.Flag[i].Num, sizeof(FlagSet.Flag[i].Num), 1, fp);
				}
				break;
			case 2:
				EveManager.Save(fp);
				break;
			}
			/////////////////////////////////////////////////////////////////////////////////
			fclose(fp);
		}
	///////////////////////////////////////////////////////////////////////////////////////////
	
	return true;
}


void CField::ChangeTextMode(bool _box, const char* _eventtext){
	if (_box){
		TextBox = &TextBox1;
	}else{
		TextBox = &TextWrap1;

		if (_eventtext!=NULL){	//EveManager::CopyOriginalEventを汎用性を上げて改善。これでTextWrap1に@EventWrapの内容を渡せた
			std::vector<char256> tmptext;
			EveManager.CopyOriginalEvent(&tmptext, _eventtext);
			for (unsigned int i=0; i<tmptext.size(); i++){
				TextWrap1.AddStock(tmptext[i].text);
			}
			TextBox->NextPage(&CmdList, &FlagSet);
		}
	}
};