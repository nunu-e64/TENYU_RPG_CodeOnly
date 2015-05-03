#include "../Define.h"
#include "Species.h"


void CSpecies::SetValue(const char* _name, int _level, int _geneMaxHp, int _geneAtk, int _geneDef, int _geneSpd) {
	bool valueChecker[4];

	Name = _name;
	Level = _level;

	MaxHpGene = between(1, 5, _geneMaxHp, &valueChecker[0]);
	AtkGene = between(1, 3, _geneAtk, &valueChecker[1]);
	DefGene = between(1, 3, _geneDef, &valueChecker[2]);
	SpdGene = between(1, 3, _geneSpd, &valueChecker[3]);

	if (!valueChecker[0]) WARNINGDX("ValueWarning(MaxHpGene):%d", _geneMaxHp);
	if (!valueChecker[1]) WARNINGDX("ValueWarning(AtkGene):%d", _geneAtk);
	if (!valueChecker[2]) WARNINGDX("ValueWarning(DefGene):%d", _geneDef);
	if (!valueChecker[3]) WARNINGDX("ValueWarning(SpdGene):%d", _geneSpd);

	Hp = CalcValue(MAXHP, _level, MaxHpGene);
}

int CSpecies::CalcValue(value_tag _key, int _level, int _gene) {

	bool checker;
	_level = between(1, 9999, _level, &checker);
	if (!checker) WARNINGDX("Name=%s, key=%d, gene=%d", Name.c_str(), (int)_key, _gene);

	switch (_key) {
	case MAXHP:
		switch (_gene) {
		case 1:
			return (_level + 4) * 10;
			break;
		case 2:
			return (_level + 4) * 12;
			break;
		case 3:
			return (_level + 4) * 14;
			break;
		case 4:
			return (_level + 4) * 17;
			break;
		case 5:
			return (_level + 4) * 20;
			break;
		default:
			ERRORDX("ErrorGene[MAXHP]: Name=%s, _key=%d,_level=,_gene=%d (instead:calc by _gene=3)", Name.c_str(), (int)_key, _level, _gene);
			return CalcValue(_key, _level, _gene);
			break;
		}
		break;
	case ATK:
	case DEF:
		switch (_gene) {
		case 1:
			return 70 + _level;
			break;
		case 2:
			return 80 + _level * 2;
			break;
		case 3:
			return 90 + _level * 3;
			break;
		default:
			ERRORDX("ErrorGene[ATKorDEF] Name=%s, _key=%d,_level=,_gene=%d (instead: calc by _gene=2)", Name.c_str(), (int)_key, _level, _gene);
			return CalcValue(_key, _level, _gene);
			break;
		}
		break;
	default:
		ERRORDX("Error value_key:%d: Name=%s, (instead: return -1)", Name.c_str(), (int)_key);
		return -1;
		break;
	}

}