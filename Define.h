////多重インクルード防止（インクルードガード）//
#ifndef DEFINE_H							////
#define DEFINE_H							////
////////////////////////////////////////////////

//#define PRODUCT_MODE  //製品版
	#ifdef PRODUCT_MODE
		#define MYLOG_DISABLE
		#define WARNINGDX_DISABLE	//マクロの有効化/無効化
		#define DEBUGDX_DISABLE	//マクロの有効化/無効化
		#define CHECK_TIME_DISABLE	//マクロの有効化/無効化
		#define MEMORY_CHECK_DISABLE //マクロの有効化/無効化
		#define FPS_DISABLE
	#endif

#define DEBUG_MODE	//プログラマデバッグ版 （他のメンバーに渡す時にはコメントアウト）
	#ifndef DEBUG_MODE
		#define DEBUGDX_DISABLE	//マクロの有効化/無効化
		#define CHECK_TIME_DISABLE	//マクロの有効化/無効化
		#define MEMORY_CHECK_DISABLE //マクロの有効化/無効化
	#endif

#include "Dxlib.h"
#include <vector>
#include <map>
#include <string>

#define MEMORY_CHECK	//メモリの解放し忘れをチェック。Debugビルドで使用可能。
	#if defined(MEMORY_CHECK) && !defined(MEMORY_CHECK_DISABLE)
		#define _CRTDBG_MAP_ALLOC
		#include <stdlib.h>
		#include <crtdbg.h>
		#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)
	#endif

#include "nunuLib.h"
//using namespace nunuLib;

//"Mrt.h"でDefine.hをインクルードしているのでMrt.hは使うとことで任意にインクルード


////////////////////////////////////////////////
#define MAP_MAX_CHECK(_mapnum, _return) {	\
	if (!(_mapnum<MAP_MAX)){				\
		ErrorDx("Error overnum[MapMax]: %d",_mapnum);return _return;	\
	}else if(_mapnum<0){					\
		ErrorDx("Error mapnum<0: %d",_mapnum);return _return;	\
	}}

#define CONSTRUCTED //myLog("MyClassLog", "%s.Constructed", typeid(*this).name())
#define DESTRUCTED //myLog("MyClassLog", "%s.Destructed", typeid(*this).name())


//戦闘関連///////////////////////////////////////////////
const int MAX_PLAYER_NUM = 3;
const int MAX_ENEMY_NUM = 3;
const int MAX_MP = 10;
const int MAX_ATTENTION = 10;
/////////////////////////////////////////////////////////

//rpgファイル名等///////////////////////////////////////////
const char FILE_B_SYSTEM[] = "tenyu_data/b_system.rpg";	//戦闘全般（エンカウント・技・敵味方ステータスなど）関するテキストファイル
const char FILE_SYSTEM[] = "tenyu_data/system.rpg";		//システム全般に関する読み込みテキストファイル
const char FILE_SCENARIO[] = "tenyu_data/scenario.rpg";	//シナリオ及びイベントに関するテキストファイル
const char FILE_EVENT[] = "tenyu_data/event.rpg";		//オリジナルイベントに関するテキストファイル
const char DIR_SAVE[] = "tenyu_data/save";				//セーブデータを保存するディレクトリ
const char FILE_DATANAME[] = "tenyu_data/save/dataname.rpg";	//セーブデータ名を管理するためのテキストファイル

//システムグラフィック///////////////////////////////////
const char IMAGE_TALKNAMELABEL[3][64] ={ "tenyu_data/pic/sys/talkname00.png",
									     "tenyu_data/pic/sys/talkname01.png",
									     "tenyu_data/pic/sys/talkname02.png"};


//システムグラフィックのPicKey/////////////////////////
const char TARGET_CURSOR[] = "TARGET_CURSOR";
const char HP_BAR[] = "HP_BAR";
const char TIME_BAR1[] = "TIME_BAR1";
const char TIME_BAR2[] = "TIME_BAR2";
const char ATTENTION_MARKER[] = "ATTENTION_MARKER";

//ゲーム名///////////////////////////////////////////
const char GAME_TITLE[] = "TENYU";
const char VERSION[] = "ver0.00";

//ゲーム内//////////////////////////////////////////////////////
const int PLAYDATA_NUM = 0;	//セーブデータの数(0~3)
const int MAP_CHIP_SIZE = 32;
const int MAP_SIZE = 128;		//正方形を想定
const int MAP_DATA_SIZE = 256;	//バイナリデータに格納されているデータの最大値（256=0~255=8bit=1byte）

const char MAP_FILE_TYPE[] = ".Map2";
const char EMAP_FILE_TYPE[] = ".eMap";

const unsigned int MAP_MAX = 2;	//読み込めるマップの最大数	//MAPデータ配列の一次の要素数
const int CHARA_PIC_NUM = 16;	//キャラの絵が一枚何コマなのか

//プログラム内//////////////////////////////////////////////////////
const char CMD_SEPARATOR[] = " ,	";
const char EOP[] = "EOP";	//EndOfParagraph

const char IFBEGIN[] = "IF_BEGIN";
const char IFEND[] = "IF_END";
const char IFCASE[] = "CASE";
const char _IFBEGIN[] = "_IF_BEGIN";
const char _IFEND[] = "_IF_END";
const char _IFCASE[] = "_CASE";
const char JOKER_NAME[] = "joker";
////////////////////////////////////////////////////

enum gamemode_tag{
	MODE_PLAYING,
	MODE_GAMEOVER,
	MODE_GAMECLEAR,
	MODE_BACKTOTITLE,
	MODE_GAMEEND,
};
enum title_tag{
	TITLE_FIRSTSTART,
	TITLE_LOADSTART,
	TITLE_SETTING,
	TITLE_GAMEEND,
	TITLE_NUM,
};
enum direction_tag{		//順番変更禁止（System.cppで使用）
	RIGHT,
	LEFT,
	DOWN,
	UP,
	DIRECTION_NUM
};
enum objkind_tag{
	PANEL,				//踏むことでイベント開始
	WALKABLE_NUM,	//↑これより上のKindはプレイヤーが上を歩くことができる（当たり判定がなく踏むことでイベント）
	BLOCK,				//BLOCK,NPCは調べることでイベント開始
	NPC,
	PUSHBLOCK,			//押すことでイベント開始（調べない）
	UNDERDRAW_NUM,	//↑これより上（数字としては小さい）のKindはプレイヤーキャラの下に描写
	COVER,				//イベント開始方法がない、下を歩行可能
	KIND_NUM
};
enum charaeffect_tag{
	NONE,
	BLINK,
	KEEP_NUM,		//↓これより下（数字としては大きい）ときはイベント発生時に、一時的にEffectがNONEになる
	RND_DIR,
	WALK,
	EFFECT_NUM
};
enum btlresult_tag{
	WIN,
	LOSE,
	LOSE_NOSCREENCHANGE
};
enum attention_tag{
	ATTENTION_DAMAGE = 2,
	ATTENIOTN_DEFFENCE = -1,
	ATTENIOTN_WAITING = -1
};


struct char256{
	char text[256];
	bool operator<(const char256& obj)const{
		return mystrgrt(text, obj.text, false);
	}
	bool operator>(const char256& obj)const{
		return mystrgrt(text, obj.text, true);
	}
};

//共用関数やクラス/////////////////////////////////
namespace sys{
	direction_tag TurnDir(int _dir, signed int _rightspin);
	direction_tag StrtoDir(const char* _str, int _originaldir=DOWN);
	bool PlayerName(const char* _str);
	bool TrueOrFalse(const char* _str, bool _torf);
	int rank3(const char* _str, int _exception=2);
	bool CheckStrNULL(const char* _str);
	bool CheckStrNULL(const std::string _str);
}
struct sideeffect_tag{
	enum{
		ATK_UP,
		ATK_DOWN,
		HEAL_ME,
		HEAL_FRIEND,
		HEAL_PARTY,
		TRICKEFFECT_NUM,
	}; int TrickEffect;
	int Power;		//効果力
	int Incidence;	//発生確率
};
struct trick_tag{
	char Name[32];
	int Power;
	int Cost;
	std::vector <sideeffect_tag> SideEffect;
	int DamageEffectIndex;

	enum targetType_tag{
		SINGLE,
		ALL,
		SINGLE_FRIEND,
		ALL_FRIEND,
		TARGETTYPE_NUM,
	}TargetType;


};

struct flag_tag{
	char Key[32];
	int Num;
};

class CFlagSet{
public:
	CFlagSet(){
		CONSTRUCTED;
	}
	~CFlagSet(){
		DESTRUCTED;
	}

	bool CreateNewFlag(const char* _key){
		for(unsigned int i=0; i<Flag.size(); i++){
			if (mystrcmp(Flag[i].Key, _key)) return false;
		}
		CreateFlag(_key, 0);
		return true;
	}

	void SetFlag(const char* _key, int _num=0, bool _add=false, bool _create=false){
		for(unsigned int i=0; i<Flag.size(); i++){
			if (mystrcmp(Flag[i].Key, _key)) {
				int num = (_add? Flag[i].Num+_num: _num);

				if (num<0) ErrorDx("Error->You can't set [num<0] for FLAG (changed to 0):%s", _key);
				Flag[i].Num=max(0,num);
				return;
			}
		}

		if (_create){
			CreateFlag(_key, _num);
		}else{
			ErrorDx("Error->Not Found Flag[%s]", _key);
		}
	};

	int GetFlagNum(const char* _key){
		for(unsigned int i=0; i<Flag.size(); i++){
			if (mystrcmp(Flag[i].Key, _key)) return Flag[i].Num;
		}
		ErrorDx("Error->Not Found Flag[%s] (return -2)", _key);
		return -2;	//未定義
	};
	
	std::vector <flag_tag> Flag;

private:
	
	bool CreateFlag(const char* _key, int _num=0){
		flag_tag newflag;
		mystrcpy(newflag.Key, _key, 32);

		if (_num<0){
			ErrorDx("Error->You can't use [num<0] for FLAG (changed to 0) :%s", _num);
			newflag.Num = 0;
		}else{
			newflag.Num = _num;
		}
		
		Flag.push_back(newflag);
		return true;
	};
};

////////////////////////////////////////////////


//セーブデータ用の構造体/////////////////////////////////
class CEveObj;
struct playdata_tag{
	bool Exist;		//セーブデータが存在するか否か

	int NowMap;
	unsigned int X, Y;
	unsigned int OldX, OldY;
	char PlayerPicKey[32];
	enum direction_tag Dir;
	int Step;	//0~3
	int Dx, Dy;
	bool Visible;

	CFlagSet FlagSet;
	std::vector<CEveObj> EveObj;

	char DataName[32];

};
///////////////////////////////////////////////////


////多重インクルード防止（インクルードガード）//
#endif										////	
////////////////////////////////////////////////