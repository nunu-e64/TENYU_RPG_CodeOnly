////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
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


////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										////
////////////////////////////////////////////////