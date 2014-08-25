////多重インクルード防止（インクルードガード）//	//pragmaonceと同じ意味だがこちらはコンパイラに依存しない
#ifndef MRT_H								////
#define MRT_H								////
////////////////////////////////////////////////

#include "mrtLib.h"
#include "Define.h"


namespace mrt{	//関数名の衝突を避けるためサブプログラマ領域の関数はすべて名前空間mrtで囲う


	int Title();
	bool Setting();
	int ChooseData(playdata_tag* _playdata);


}
////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////