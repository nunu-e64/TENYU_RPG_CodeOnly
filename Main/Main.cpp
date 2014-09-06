#include "../Define.h"
#include "Main.h"

#include "../Mrt.h"
#include "../Field/Field.h"
#include "../Field/Load.h"


bool CMain::Init(){
	srand((unsigned int)GetNowCount());
	//ChangeFont("ＭＳ Ｐゴシック") ;	$
	return true;
}

bool CMain::GameLoop(){
	int dnum;

	StartDemo();

	{
		CLoad PlayDataLoad;
			char path[256]; strcpy_s(path, "tenyu_data/save");
			PlayDataLoad.LoadPlayData(path, PlayData);
	}

	while(1){
		switch(Title()){
		case TITLE_FIRSTSTART:
			Opening();
			return Playing();

		case TITLE_LOADSTART:
			if ((dnum = ChooseData()) != -1){
				return Playing(dnum);
			}
			break;

		case TITLE_SETTING:
			Setting();
			break;
		
		case TITLE_GAMEEND:
			return false;

		default:
			WarningDx("Warning->Unexpected matter happend [back to title]->GameLoop()", __FILE__, __LINE__);
		}
	}

	return true;
}

bool CMain::Playing(const int _dnum){
	//DebugDx("PlayingStart");
	CField Field;
	//DebugDx("PlayingMiddle");
	if (!Field.Init(PlayData, _dnum)) return true;
	//DebugDx("Playingok");

	switch (Field.MainLoop()){
	case MODE_GAMEOVER:
		return GameOver();
	case MODE_GAMECLEAR:
		return GameClear();
	case MODE_BACKTOTITLE:
		return true;
	case MODE_GAMEEND:
		return false;
	default:
		WarningDx("Warning->Unexpected matter happend [back to title]->Playing(int_dnum)", __FILE__, __LINE__);
		return true;
	}
}

int CMain::Title(){
	return mrt::Title();
}

int CMain::ChooseData(){		//セーブデータの選択画面
	return mrt::ChooseData(PlayData);
}

void CMain::StartDemo(){
	int timecount = 0;
	int scene = 0;
	
	CheckHitKeyDown(KEY_INPUT_OK);	//キー状態をリセット
	while( BasicLoop() && !CheckHitKeyDown(KEY_INPUT_ESCAPE) && !CheckHitKeyDown(KEY_INPUT_OK)  && !CheckHitKeyDown(KEY_INPUT_CANCEL) && scene != -1){
		
		switch(scene){
		case 0:
			if (timecount<255) {
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, timecount);
			}else if(timecount<255*2){
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255-(timecount-255));
			}else if(timecount==255*2){
				scene = -1;
				break;
			}
			DrawCenterString(WINDOW_WIDTH/2, (int)(WINDOW_HEIGHT*2/5), "Opening Demo", WHITE);
			DrawCenterString(WINDOW_WIDTH/2, (int)(WINDOW_HEIGHT*4/5), "Presented by MathLabo OBOG in IH", WHITE);
			break;
		}
		
		timecount++;
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
};


void CMain::Opening(){
	int timecount = 0;
	int scene = 0;
	int oldfontsize = GetFontSize();
	int fontsize = 14;
	char* optext[] = {"物語の舞台は島国ソラリシア",
		"この国にはかつて太陽の神と星の神を奉る二神教が存在し",
		"信者の中には信仰を捧げた神から魔力を授かるものもいた",
		"昼の太陽から力を授かる“太陽の魔法使い”と",
		"夜の星から力を授かる“星の魔法使い”である",
		"しかし、急速な発達が進む都市部では信仰は薄れ",
		"ソラリシア王によって新たな宗教が興される",
		"弾圧された旧教徒たちは、格差にあえぐ人々と手を組み反乱の狼煙を上げた",
		"それから30年、この国には今なお争乱の火種がくすぶり続けている",
		"そして今、レジスタンスに新たなリーダーが誕生する",
		"これは、長きにわたる紛争に新たな歴史を刻む、若きリーダーの英雄譚である（ここまですべて嘘）"};
	
	SetFontSize(fontsize);
	while( BasicLoop() && !CheckHitKeyDown(KEY_INPUT_ESCAPE) && !CheckHitKeyDown(KEY_INPUT_OK) && scene != -1){
		
		switch(scene){
		case 0:
			if(timecount==3300){
				scene = -1;
			}
			for (int i=0; i<11; i++){
				DrawCenterString(WINDOW_WIDTH/2, (int)(WINDOW_HEIGHT-timecount/3) + i*(fontsize*4), WHITE, optext[i]);
			}
			break;
		}
		
		timecount++;
	}
	
	SetFontSize(oldfontsize);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
};


bool CMain::GameOver(){
	int timecount=0;

	while( BasicLoop()){
		if (timecount<255) {
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, timecount);
		}else{	
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		
			if (int(timecount/40)%2) DrawCenterString(WINDOW_WIDTH/2, (int)(WINDOW_HEIGHT*4/5), "Z→タイトルへ戻る, X→ゲーム終了", GRAY);
		}
		
		DrawCenterString(WINDOW_WIDTH/2, (int)(WINDOW_HEIGHT*2/5), "GAME OVER", WHITE);
		timecount++;

		if (CheckHitKeyDown(KEY_INPUT_OK)){
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	
			return true;
		}else if (CheckHitKeyDown(KEY_INPUT_CANCEL) || CheckHitKeyDown(KEY_INPUT_ESCAPE)){
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	
			return false;
		}
	}
	WarningDx("Warning->Unexpected error happend[program_end]: GameOver()", __FILE__, __LINE__);
	return false;
};

bool CMain::GameClear(){
	int timecount=0;

	while( BasicLoop() ){
		if (timecount<255) {
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, timecount);
		}else{	
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);		
			if (int(timecount/40)%2) DrawCenterString(WINDOW_WIDTH/2, (int)(WINDOW_HEIGHT*4/5), "Z→タイトルへ戻る, X→ゲーム終了", GRAY);
		}
		
		DrawCenterString(WINDOW_WIDTH/2, (int)(WINDOW_HEIGHT*2/5), "GAME CLEAR!", WHITE);
		timecount++;

		if (CheckHitKeyDown(KEY_INPUT_OK)){
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	
			return true;
		}else if (CheckHitKeyDown(KEY_INPUT_CANCEL) || CheckHitKeyDown(KEY_INPUT_ESCAPE)){
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	
			return false;
		}
	}

	WarningDx("Error->Unexpected matter happend[program_end]: GameClear()", __FILE__, __LINE__);
	return false;
}

void CMain::Setting(){
	int oldfontsize = GetFontSize();
	
	while(mrt::Setting()){
		//設定処理
	}

	SetFontSize(oldfontsize);
}