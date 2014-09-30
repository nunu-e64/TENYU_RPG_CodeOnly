////多重インクルード防止（インクルードガード）//
#ifndef ENEMYSPECIESMANAGER_H				////
#define ENYMYSPECIESMANAGER_H				////
////////////////////////////////////////////////

#include <map>
#include <string>
#include "EnemySpecies.h"



class CEnemySpeciesManager{
public:
	CEnemySpeciesManager(void);

	bool CreateEnemySpecies(const char* _name, int _maxhp, int _atk, int _def, int _spd);
	bool SetTrickList(const char* _name, std::vector <trick_tag const*> _trickList);
	bool SetImg(const char* _name, int _img);

	CEnemySpecies GetEnemySpecies(const char* _name);

private:
	std::map <std::string, CEnemySpecies> EnemyBank;

};


////多重インクルード防止（インクルードガード）//
#endif										  //
////////////////////////////////////////////////