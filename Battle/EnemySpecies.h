////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
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

	std::vector <trick_tag const*> TrickList;		//�Z���X�g

};


////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										////
////////////////////////////////////////////////
