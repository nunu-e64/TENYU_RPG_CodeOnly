////多重インクルード防止（インクルードガード）//
#ifndef SPECIES_H							////
#define SPECIES_H							////
////////////////////////////////////////////////


class CSpecies{
public:
	CSpecies(){
		CONSTRUCTED;
		Name = "NULL_NAME";
		Lv = Img = Atk = Def = Spd = -1;
		Hp = MaxHp = 1;
	}
	~CSpecies(){DESTRUCTED;}

	std::string GetName()const{return Name;}

protected:
	std::string Name;
	int Img;

	int Lv;
	int Hp;
	int MaxHp;
	int Atk;
	int Def;
	int Spd;		//%→相対値
	
	std::vector <trick_tag const*> TrickList;		//技リスト

	void SetValue(const char* _name, int _maxhp, int _atk, int _def, int _spd);

};


class CPlayerSpecies : public virtual CSpecies{
	friend class CPlayerSpeciesManager;
public:
	CPlayerSpecies(const CPlayerSpecies& obj){
		CONSTRUCTED;
		*this = obj;	//これが必須なのが不思議。何故かデフォルトコピーコンストラクタが仕事してくれない。
	}
	CPlayerSpecies(){
		CONSTRUCTED;
		Exp=0;
	}
	~CPlayerSpecies(){
		DESTRUCTED;
	}

private:
	int Exp;
};


#include "EnemyAI.h"

class CEnemySpecies : public virtual CSpecies{
	friend class CEnemySpeciesManager;
public:
	CEnemySpecies(const CEnemySpecies& obj){
		CONSTRUCTED;	
		*this = obj;	//これが必須なのが不思議。何故かデフォルトコピーコンストラクタが仕事してくれない。
	}
	CEnemySpecies(){
		CONSTRUCTED;
	}
	~CEnemySpecies(){
		DESTRUCTED;	
	}

protected:
	CEnemyAI AI;

private:
	int GoldGene;
	int ExpGene;
	std::map <int, std::vector<std::pair<int, int> > > RandomPlanSet;
};


////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////
