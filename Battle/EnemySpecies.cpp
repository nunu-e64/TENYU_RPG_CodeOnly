#include "../Define.h"
#include "EnemySpecies.h"


CEnemySpecies::CEnemySpecies(void)
{
}


CEnemySpecies::~CEnemySpecies(void)
{
}

void CEnemySpecies::SetValue(const char* _name, int _maxhp, int _atk, int _def, int _spd){
	
	Name = _name;

	MaxHp = _maxhp;
	Atk = max(1,_atk);
	Def = max(1,_def);
	Spd = max(1,_spd);

}


