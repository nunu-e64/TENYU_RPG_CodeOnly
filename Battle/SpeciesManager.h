////多重インクルード防止（インクルードガード）//
#ifndef SPECIESMANAGER_H				////
#define SPECIESMANAGER_H				////
////////////////////////////////////////////////

#include <map>
#include <string>
#include "Species.h"

class CPlayerSpeciesManager{
public:
	CPlayerSpeciesManager(){};

	bool CreateSpecies(const char* _name, int _maxhp, int _atk, int _def, int _spd);
	bool SetTrickList(const char* _name, std::vector <trick_tag const*> _trickList);
	bool SetImg(const char* _name, int _img);

	CPlayerSpecies GetSpecies(const char* _name);
	
private:
	std::map <std::string, CPlayerSpecies> PlayerBank;

};


class CEnemySpeciesManager{
public:
	CEnemySpeciesManager(){};

	bool CreateSpecies(const char* _name, int _maxhp, int _atk, int _def, int _spd);
	bool SetTrickList(const char* _name, std::vector <trick_tag const*> _trickList);
	bool SetImg(const char* _name, int _img);

	CEnemySpecies GetSpecies(const char* _name);

private:
	std::map <std::string, CEnemySpecies> EnemyBank;

};


////多重インクルード防止（インクルードガード）//
#endif										  //
////////////////////////////////////////////////