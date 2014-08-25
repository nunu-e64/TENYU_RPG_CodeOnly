////多重インクルード防止（インクルードガード）//	//pragmaonceと同じ意味だがこちらはコンパイラに依存しない
#ifndef NUNULIB_H							////
#define NUNULIB_H							////
////////////////////////////////////////////////

#include "DxLib.h"
#include "math.h"
#include <direct.h>

//#define WARNINGDX_DISABLE	//マクロの無効化
//#define DEBUGDX_DISABLE	//マクロの無効化
#define CHECK_TIME_DISABLE	//時間測定マクロの無効化
	#define CHECK_TIME2_DISABLE	//時間測定マクロ（狭い）の無効化
#define ARRAY_SIZE(array)    (sizeof(array)/sizeof(array[0]))

const double PI = 3.1415926535897932384626433832795f;

////////////////////////////////////////////////////////
//色定数////////////////////////////////////////////////
	const int RED = GetColor( 255 , 0 , 0 );
	const int GREEN = GetColor( 0 , 255 , 0 );
	const int BLUE = GetColor( 0 , 0 , 255 );
	const int YELLOW = GetColor( 255 , 255 , 0 );
	const int MAGENTA = GetColor(255, 0, 255);
	const int BLACK = GetColor( 0 , 0 , 0 );
	const int WHITE = GetColor( 255 , 255 , 255 );
	const int GRAY = GetColor( 120 , 120 , 120 );
////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
///時間測定関数//////////////////////////////////////////////
	#ifdef CHECK_TIME_DISABLE
	#define CHECK_TIME_START    /* 何もしない */
	#else
	#define CHECK_TIME_START {							\
			int check_perform_start = GetNowCount();	
	#endif

	#ifdef CHECK_TIME_DISABLE
	#define CHECK_TIME_END(title)    /* 何もしない */
	#else
	#define CHECK_TIME_END(title)					\
			int check_perform_end = GetNowCount();		\
			printfDx( "%s: %dms\n", title, (check_perform_end - check_perform_start)); \
			ScreenFlip();	\
			WaitKey();	\
		}	
	#endif

	#ifdef CHECK_TIME2_DISABLE
	#define CHECK_TIME_START2    /* 何もしない */
	#else
	#define CHECK_TIME_START2 {							\
			int check_perform_start = GetNowCount();	
	#endif

	#ifdef CHECK_TIME2_DISABLE
	#define CHECK_TIME_END2(title)    /* 何もしない */
	#else
	#define CHECK_TIME_END(title)					\
			int check_perform_end = GetNowCount();		\
			printfDx( "%s: %dms\n", title, (check_perform_end - check_perform_start)); \
			ScreenFlip();	\
			WaitKey();	\
		}	
	#endif
/////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////
///二次元座標用クラス////////////////////////////////////////
class CVector{	//宣言と同時に定義を書いているのはインライン化
public:
	CVector( double _x , double _y ){
		x = _x;
		y = _y;
	}

	CVector(){
		x = 0;
		y = 0;
	}

	double x;
	double y;

	void Set( double _x , double _y){
		x = _x;
		y = _y;
	}
	void Set(CVector _vec){
		x = _vec.x;
		y = _vec.y;
	}
	
	CVector Add( double _x , double _y){
		x += _x;
		y += _y;
		return *this;
	}
	CVector Add(CVector _vec){
		x += _vec.x;
		y += _vec.y;
		return *this;
	}

	// +演算子オーバーロード
	CVector operator+(const CVector &obj){
		CVector tmp;
		tmp.x = x + obj.x;
		tmp.y = y + obj.y;
		return tmp;
	}

	// +=演算子オーバーロード
	CVector& operator+=(const CVector &obj){
		x += obj.x;
		y += obj.y;
		return *this;
	}
	CVector& operator+=(const double _num){
		x += _num;
		y += _num;
		return *this;
	}
	
	// -演算子オーバーロード
	CVector operator-(const CVector &obj){
		CVector tmp;
		tmp.x = x - obj.x;
		tmp.y = y - obj.y;
		return tmp;
	}
	// -=演算子オーバーロード
	CVector& operator-=(const CVector &obj){
		x -= obj.x;
		y -= obj.y;
		return *this;
	}
	CVector& operator-=(const double _num){
		x -= _num;
		y -= _num;
		return *this;
	}
	//*演算子オーバーロード
	CVector operator*(const double _num){
		CVector tmp;
		tmp.x = x * _num;
		tmp.y = y * _num;
		return tmp;
	}
	//*=演算子オーバーロード
	CVector& operator*=(const double _num){
		x*=_num;
		y*=_num;
		return *this;
	}

	//operator*のオーバーロードによりもはや不要
	void Multiple(double _num){
		x = x*_num;
		y = y*_num;
	}

	double GetLength(){
		return sqrt((x*x)+(y*y));
	}
private:
};
/////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////
////文字列系///////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
////文字列を数値に変換///////////////////////////////////////
bool mystrtol(const char* str, long int* result, int radix=10);
bool mystrtol(const char* str, int* result, int radix=10);
	//atoiにはエラー判定がないためこの関数を利用推奨
	//	str		：	変換対象の文字列
	//	*result	：	変換したLong型の数を受け取るアドレス　←int型も可に
	//	radix	：	基数。数値を何進数とみなすかを設定。デフォルトは10進数
/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
////strlenをsignedに/////////////////////////////////////////
signed int mystrlen(const char* _str);				   //洋梨
/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
///文字列比較用関数//////////////////////////////////////////
bool mystrcmp(const char *String, const char *Words, const char Option='p');
bool mystrcmp(const char *String, const char Option, const int arg_num, ...);		//複数キーワードをOR判定
	//	'p'	：完全一致：	Wordsが""の時、False
	//	'l'	：左方一致：	Wordsが""の時、True
	//	'm'	：部分一致：	Wordsが""の時、True
/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
///半角文字正確探索用関数//////////////////////////////////////
char* mystrchr(char* string, char character);
/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
///文字列コピー用関数////////////////////////////////////////
bool mystrcpy(char *String, const char *Source);
bool mystrcpy(char **String, const char **Source, const int SourceLineSize);
bool mystrcpy(char *String, const char *Source, int size_including_null);
/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
///文字列の両端から余計な文字を削る関数//////////////////////
bool mystrsmt(char *string, const char* uselesswords);
/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
///文字列結合用関数////////////////////////////////////////
bool mystrcat(char *string, const char* source);
/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
///文字列置換用関数////////////////////////////////////////
void mystrrep(char* string, const char* from, const char* to);
/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
///文字列大小比較用関数(greater)//////////////////////////////
bool mystrgrt(const char* string, const char* target, bool greater=true);
/////////////////////////////////////////////////////////////

////文字列系ここまで/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////
////描画系//////////////////////////////////////////////////////////////////////////
CVector GetGraphSize(int GrHandle);		//画像のサイズをCVector型で返す
int LoadGraph(const TCHAR *filename, bool errorcheck);		//DxライブラリのLoadGraphのファイルエラー出力付き版。コード節約が目的。
inline int DrawGraph(double x, double y, int GrHandle, int TransFlag){		//DxライブラリのDrawGraphでdoubleをintに自動キャストしてくれるだけ
	return DrawGraph((int)x, (int)y, GrHandle, TransFlag);
}
inline int DrawExtendGraph(double x1, double y1, double x2, double y2, int GrHandle, int TransFlag){		//DxライブラリのDrawExtendGraphでdoubleをintに自動キャストしてくれるだけ
	return DrawExtendGraph((int)x1, (int)y1, (int)x2, (int)y2, GrHandle, TransFlag);
}
inline int DrawTurnGraph(double x, double y, int GrHandle, int TransFlag){		//DxライブラリのDrawTurnGraphでdoubleをintに自動キャストしてくれるだけ
	return DrawTurnGraph((int)x, (int)y, GrHandle, TransFlag);
}

int DrawString(int x, int y, int color, const TCHAR* format, ...);	//フォーマット対応
inline int DrawCenterString(int x, int y, const TCHAR *String, int color){	//xを左右の中心にしてDrawStringで文字描画（※yは上下中心ではなく上辺）
	return DrawString(x-GetDrawStringWidth(String, strlen(String))/2, y, String, color); 
}
int DrawCenterString(int x, int y, int color, const TCHAR* format, ...);
////描画系ここまで//////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
///エラー&デバッグ出力用関数/////////////////////////////////
template<class T>inline void ErrorDx(const char* format, char* filename, int line, const T& a){
	printfDx(format, a);
	printfDx("\n->%s(%d)\n", filename, line);
	ScreenFlip();
	WaitKey();
	clsDx();
	ClearDrawScreen();
}
template<class T>inline void ErrorDx(const char* format, const T& a){
	printfDx(format, a);
	printfDx("\n");
	ScreenFlip();
	WaitKey();
	clsDx();
	ClearDrawScreen();
}
inline void ErrorDx(const char* format, char* filename, int line){
	ErrorDx(format,filename,line,0);
}
inline void ErrorDx(const char* format){
	ErrorDx(format,0);
}

template<class T>inline void WarningDx(const char* format, char* filename, int line, const T& a){
	#ifndef	WARNINGDX_DISABLE 
		ErrorDx(format, filename, line, a);
	#endif
}
template<class T>inline void WarningDx(const char* format, const T& a){
	#ifndef	WARNINGDX_DISABLE 
		ErrorDx(format, a);
	#endif
}
inline void WarningDx(const char* format, char* filename, int line){
	#ifndef	WARNINGDX_DISABLE 
		ErrorDx(format, filename, line);
	#endif
}
inline void WarningDx(const char* format){
	#ifndef	WARNINGDX_DISABLE 
		ErrorDx(format);
	#endif
}
template<class T>inline void DebugDx(const char* format, char* filename, int line, const T& a){
	#ifndef	DEBUGDX_DISABLE 
		ErrorDx(format, filename, line, a);
	#endif
}
template<class T>inline void DebugDx(const char* format, const T& a){
	#ifndef	DEBUGDX_DISABLE 
		ErrorDx(format, a);
	#endif
}
inline void DebugDx(const char* format, char* filename, int line){
	#ifndef	DEBUGDX_DISABLE 
		ErrorDx(format, filename, line);
	#endif
}
inline void DebugDx(const char* format){
	#ifndef	DEBUGDX_DISABLE 
		ErrorDx(format);
	#endif
}
/////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////
//////自作Choose関数/////////////////////////////////////////
int choose(const int target, ...);	//int限定	//VB6仕様（一個目を指定したい→target=1）
/////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////
///KeyDown判定用関数/////////////////////////////////////////
const int KEY_INPUT_OK = -1;
const int KEY_INPUT_CANCEL = -2;

namespace nunuLib{			//クラス名衝突を避けるため名前空間に包む
	class CKeyManager{		//構造体変数にしてもいいけどなるべくstaticを使いたくなかった
	public:
		CKeyManager();
		bool CheckDown(const int KEY_CODE);
	private:
		bool Pressed[256];
	};
}
bool CheckHitKeyDown(const int KEY_CODE); //※決定キー･･･Z,Enter　キャンセルキー･･･X,BackSpace　それぞれどちらでも可
/////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////
//////フォーマット対応ウィンドウタイトル変更関数/////////////
int SetTitle(const char* format, ...);		//Dxlib関数SetMainWindowTextの上位互換
/////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////
////その他///////////////////////////////////////////////////
template<class T>inline T between(const T& min_border, const T& max_border, const T& num)	//numがmin_border以上max_border以下になるようチェック	※引数はすべて型が同じでなくてはならない
{	return max((min_border), min((max_border), (num)));
}
inline int mod(int a, int b){	//VB6のように必ず非負のあまりを返す関数。%演算子は負の数を取りうるので不便。
	return ((b)+((a)%(b)))%(b);
}
inline bool BasicLoop(){
	if(ScreenFlip()==0 && ProcessMessage()==0 && ClearDrawScreen()==0){
		return true;
	}else{ return false;}
}
/////////////////////////////////////////////////////////////



////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////