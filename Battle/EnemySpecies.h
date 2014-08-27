////多重インクルード防止（インクルードガード）//
#ifndef ENEMYSPECIES_H							////
#define ENYMYSPECIES_H							////
////////////////////////////////////////////////

#include <string>
#include <vector>


class CEnemySpecies{
public:
	CEnemySpecies(void);
	~CEnemySpecies(void);

private:
	std::string Name;
	
	int MaxHp;
	int Atk;
	int Def;

	std::vector <trick_tag const*> TrickList;		//技リスト

};


////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////
