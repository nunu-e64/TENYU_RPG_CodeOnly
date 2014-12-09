////多重インクルード防止（インクルードガード）//
#ifndef ENEMY_H								////
#define ENEMY_H								////
////////////////////////////////////////////////

#include "Actor.h"
#include "Species.h"


class CEnemy : public CActor, public CEnemySpecies{
public:
	
	CEnemy(){
		CONSTRUCTED
	};
	CEnemy(const CEnemySpecies& obj) : CEnemySpecies(obj){
		CONSTRUCTED
	};
	~CEnemy(){
		DESTRUCTED
		myLog("%s:%s:CEnemy_Destructed", typeid(*this).name(), Name.c_str());
	}

	void BattleReady(const CActor* const* _actorList, const int _playerNum, const int _enemyNum)const{
		AI->BattleReady(_actorList, _playerNum, _enemyNum);
	}

	void Draw(int _dx=0, int _dy=0);


private:
	//メンバ関数
		bool Plan();
		bool Action();

	//メンバ変数
		int Attention[MAX_PLAYER_NUM];
};



////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////
