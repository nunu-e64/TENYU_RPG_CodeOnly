////多重インクルード防止（インクルードガード）//
#ifndef ENEMYSPECIES_H							////
#define ENYMYSPECIES_H							////
////////////////////////////////////////////////

#include "Actor.h"


class CEnemySpecies : public CSpecies{
	friend class CEnemySpeciesManager;

public:
	CEnemySpecies();
	~CEnemySpecies();

	void SetValue(const char* _name, int _maxhp, int _atk, int _def, int _spd);

private:
	
};


////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////
