////多重インクルード防止（インクルードガード）//
#ifndef PLAYER_H							////
#define PLAYER_H							////
////////////////////////////////////////////////

#include "Actor.h"
#include "../Menu.h"


class CPlayer : public CActor, public CPlayerSpecies{
public:
	CPlayer(){
		CONSTRUCTED;
	}
	CPlayer(const CPlayerSpecies& obj) : CPlayerSpecies(obj){
		CONSTRUCTED;
	};
	~CPlayer(){DESTRUCTED;}
	
	void SetExtraImg(CBImgBank* _bImgBank);
	void CreateBattleMenu();
	void Draw(int _dx=0, int _dy=0);

private:
	//メンバ関数
		bool Plan();
		bool Action();
		
		double CalcDamage(double _damage, CActor* _attacker, trick_tag const* _trick);

	//メンバ変数
		int MagicCounterImg[2];

		CBattleMenu BattleMenu;

};


////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////