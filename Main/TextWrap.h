////多重インクルード防止（インクルードガード）//
#ifndef TEXTWRAP_H							////
#define TEXTWRAP_H							////
////////////////////////////////////////////////

#include "TextBox.h"

class CTextWrap : public CTextBox{	//privateな継承では親クラスのpublicメンバに直接（インスタンス.関数()の形で）アクセスできず、インスタンスを親クラスとして扱うこともできない
public:
	CTextWrap(){};
	void Term(CCmdList* _cmdlist);	//Terminate
	void Draw(bool _showingstop=false);

private:
	//行間は定数化すべきか（オーバーライドできるの？→コンストラクタで初期化すれば可能なはず）$
		
};


////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////