#include "Define.h"
#include "Mrt.h"


using namespace mrtlib;		//usingディレクティブを使うかどうかはお任せします

namespace mrt{		//usingとの位置関係がこれであってるのかは不明

int Title(){	//返り値：title_tag  引数：なし	
	/*///////////////////////////////////////
	下記を参考にZキーを押したときに適切な値を返してください
	enum title_tag{
		TITLE_LOADSTART,
		TITLE_FIRSTSTART,
		TITLE_SETTING,
		TITLE_GAMEEND,
		TITLE_NUM,
	};
	////////////////////////////////////////*/

	int target = 0;
	int timecount = 0;
	
	while( BasicLoop() && !CheckHitKeyDown(KEY_INPUT_ESCAPE)){
		
		if (CheckHitKeyDown(KEY_INPUT_DOWN)){
			target = mod(target+1, TITLE_NUM);
		}else if (CheckHitKeyDown(KEY_INPUT_UP)){
			target = mod(target-1, TITLE_NUM);
		}else if (CheckHitKeyDown(KEY_INPUT_OK)){
			return target;
		}

		timecount++;

		DrawCenterString(WINDOW_WIDTH/2, (int)(WINDOW_HEIGHT*1/10), "- 天 佑 -" , WHITE );
		DrawCenterString(WINDOW_WIDTH/2, (int)(WINDOW_HEIGHT*5/10), "つづきから", WHITE);
		DrawCenterString(WINDOW_WIDTH/2, (int)(WINDOW_HEIGHT*6/10), "はじめから", WHITE);
		DrawCenterString(WINDOW_WIDTH/2, (int)(WINDOW_HEIGHT*7/10), "　設定　　", WHITE);
		DrawCenterString(WINDOW_WIDTH/2, (int)(WINDOW_HEIGHT*8/10), "ゲーム終了", WHITE);
		
		if (int(timecount/15)%2) DrawCenterString(WINDOW_WIDTH/2, (int)(WINDOW_HEIGHT*(target+5)/10), "|>　　　　　　　", WHITE);
	}

	return TITLE_GAMEEND;
}


int ChooseData(playdata_tag* _playdata){	//返り値：ロードするデータ番号（エラー：-1[backtotitle]）  引数：セーブデータ配列
	
	/*///////////////////////////////////////
	Existがtrueのセーブデータのみ表示してください。（playdata_tagの仕様についてはDefine.h参照）
	セーブデータはPLAYDATA_NUM個あります。（_playdata[0]~_playdata[PLAYDATA_NUM-1]）
	////////////////////////////////////////*/

	int dnum = 0;
	char tmpstring[256];

	while( BasicLoop()){
		sprintf_s(tmpstring, "データ番号：%d データ名：%s", dnum, (_playdata[dnum].Exist?_playdata[dnum].DataName:"No Data")); 
		DrawCenterString(WINDOW_WIDTH/2, (int)(WINDOW_HEIGHT*1/4), "ロードするデータを選択", WHITE);
		DrawCenterString(WINDOW_WIDTH/2, (int)(WINDOW_HEIGHT*2/4), tmpstring, (_playdata[dnum].Exist? WHITE:GRAY));
		DrawCenterString(WINDOW_WIDTH/2, (int)(WINDOW_HEIGHT*3/4), "↑↓:データ選択　Z:決定　Esc:タイトルに戻る", WHITE);

		if (CheckHitKeyDown (KEY_INPUT_DOWN)){
			dnum = mod(dnum+1, PLAYDATA_NUM);
		}else if (CheckHitKeyDown (KEY_INPUT_UP)){
			dnum = mod(dnum-1, PLAYDATA_NUM);
		}

		if (CheckHitKeyDown(KEY_INPUT_OK)){
			if (_playdata[dnum].Exist){
				return dnum;
			}

		}else if (CheckHitKeyDown(KEY_INPUT_ESCAPE)){
			return -1;
		}
	}

	WarningDx("Warning->Unexpected Error Happened", __FILE__, __LINE__);
	return -1;
}

bool Setting(){	//返り値…設定変更(true) 設定終了(false)  引数…いまのところなし
	
	/*///////////////////////////////////////
		詳細未定。仕様はこれから詰める。
		音量上げ下げくらいならこの関数内で処理してもらってもいいかも…？
	////////////////////////////////////////*/

	int fontsize = 10;

	SetFontSize(fontsize);
	while( BasicLoop() && !CheckHitKeyDown(KEY_INPUT_ESCAPE)){
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);	
		DrawCenterString(WINDOW_WIDTH/2, (int)(WINDOW_HEIGHT*2/5), "各種設定（音量調整、テキストスピード調整、デザイン変更(できれば)、あと何？）をできるようにする。Escで終了。", WHITE);
	}

	return false;

}


}	//namespace閉じる