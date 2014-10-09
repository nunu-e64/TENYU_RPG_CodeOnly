#include "../Define.h"
#include "Species.h"


void CSpecies::SetValue(const char* _name, int _maxhp, int _atk, int _def, int _spd){
	
	Name = _name;

	Hp = MaxHp = _maxhp;
	Atk = max(1,_atk);
	Def = max(1,_def);
	Spd = max(1,_spd);
}
