////多重インクルード防止（インクルードガード）//
#ifndef PLAYER_H							////
#define PLAYER_H							////
////////////////////////////////////////////////

#include "Actor.h"
#include "../Menu.h"


class CPlayer : public CActor, public CPlayerSpecies{
public:
	CPlayer(){CONSTRUCTED; Mc=0;}
	CPlayer(const CPlayerSpecies& obj) : CPlayerSpecies(obj){CONSTRUCTED; Mc=0;};
	~CPlayer(){DESTRUCTED;}
	
	void CreateBattleMenu();
	void Draw(int _dx=0, int _dy=0);

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