////多重インクルード防止（インクルードガード）//
#ifndef ENEMYSPECIESMANAGER_H							////
#define ENYMYSPECIESMANAGER_H							////
////////////////////////////////////////////////

#include <map>
#include <string>
#include "EnemySpecies.h"


class CEnemySpeciesManager{
public:
	CEnemySpeciesManager(void);


private:
	std::map <std::string, CEnemySpecies> EnemyBank;
	CEnemySpecies EnemyS_dammy;

};


////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////