#include "../Define.h"
#include "Species.h"


void CSpecies::SetValue(const char* _name, int _maxhp, int _atk, int _def, double _spd){
	
	Name = _name;

	Hp = MaxHp = _maxhp;
	Atk = max(1,_atk);
	Def = max(1,_def);
	Spd = between(0.001, 100.0, (double)_spd);
}
