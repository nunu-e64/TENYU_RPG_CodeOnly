////多重インクルード防止（インクルードガード）//
#ifndef LOGWINDOW_H							////
#define LOGWINDOW_H							////
////////////////////////////////////////////////


class CCmdList;

class CLogWindow{
public:
	CLogWindow();
	~CLogWindow(){
		DESTRUCTED;
	};

	void Init(int _posx, int _posy, int _width, int _height, int _boxColor, int _line , int _word, int _fontSize, int _fontColorMain, int _fontColorSub, int _autoPlaySpeed);
	void Clear();	//表示テキスト全消去
	void Term();	//Terminate
	
	void Draw();
	
	bool Add(char *_newText);
	bool Add(char **_newTextArray);

	void SetVisible(bool _visible){Visible = _visible;}
	
private:
	//定数
		enum{
			//STOCK_LINE_NUM = 1000,
			LINE_MAX = 255,		//行数最大値		　Initで調整される
			WORD_MAX = 255,		//一行の文字数最大値　Initで調整される
			LINE_SPACE = 5,	//行間
		};

	//メンバ関数



	//メンバ変数
		bool Initialized;	//初期済みか否か

		char** Text;
		bool Visible;	//コマンドから変更可能

		/*char OldText[LINE_MAX][WORD_MAX];
		char DrawText[LINE_MAX][WORD_MAX];
*/
		int PosX, PosY, Width, Height;			//左上の座標、横幅縦幅
		int LineNum, WordNum;	//WordNumの単位はバイト　LineNumは行数
		int FontSize;
		int FontColorMain, FontColorSub;	//Color2は影
		int BoxColor;
		int WordWidth;		//WordNumとFontSizeに基づいて横幅を算出

		int NextLine;	//次に書き換えられるべきTextの配列インデックス

		/*
		int StockLine;		//何行目までデータがストックされているか				1~STOCK_LINE_NUM	空：0	//AddTextで初期化するのでNoProblem
		int TextLineNum();		//今、何行目まで表示用テキストがあるか		1~LineNum	空：0
		
		int NowStock;	//今、ストックの何行目にいるか	0~	空：-1	(TextBox外においては次調べる行はどこか)
		int NowTarget;	//今、テキストボックスの何行目について処理しているのか　0~	空：-1
		bool PageChange;	//次にページリセットするか否か

		////テキストのアニメーション表示関係//////////////
			int NewText;		//chTextに新たに追加された行　-1･･･変化なし　0~･･･上から何行目以降が追加されたか（空白もカウント）
			bool Showing;		//テキストアニメーションが現在進行中：True　完了：False
			int ShowingTime; 

		////AddTextのときに変更
			int ObjCount;
			direction_tag OriginalDir;

		//ルビ用
			struct ruby_tag{
				char Word[32];
				char Ruby[32];
				char chNum[3];	//00~99
			};
			std::vector <ruby_tag> Ruby;
			*/
};


////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////