////多重インクルード防止（インクルードガード）//
#ifndef PLAYER_H							////
#define PLAYER_H							////
////////////////////////////////////////////////

#include "Actor.h"
#include "../Menu.h"


class CPlayer : public CActor{
public:
	CPlayer(){Mc=0;}
	CPlayer(const CSpecies& _species) : CActor(_species){Mc=0;};

	void Init();
	void Draw(int dx=0, int dy=0);

private:
	//メンバ関数
		bool Plan();
		bool Action();

	//メンバ変数
		int Mc;	//MagicCount?

		CBattleMenu BattleMenu;

};


////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////