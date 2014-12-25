////多重インクルード防止（インクルードガード）//
#ifndef SPECIES_H							////
#define SPECIES_H							////
////////////////////////////////////////////////


class CSpecies{
public:
	CSpecies(){
		CONSTRUCTED;
		Name = "UNDEFINED_NAME";
		Img = -1;
		Lv = -1;
		
		Hp = MaxHp = 1;
		Atk = Def = Spd = -1;
		
		TrickList.clear();
	}
	~CSpecies(){DESTRUCTED;}

	std::string GetName()const{return Name;}

protected:
	void SetValue(const char* _name, int _maxhp, int _atk, int _def, int _spd);

	std::string Name;
	int Img;

	int Lv;
	int Hp;
	int MaxHp;
	int Atk;
	int Def;
	int Spd;		//%
	
	std::vector <trick_tag const*> TrickList;		//技リスト
	
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
#include <vector>

class CEnemySpecies : public virtual CSpecies{
	friend class CEnemySpeciesManager;
public:
	CEnemySpecies(const CEnemySpecies& obj){
		CONSTRUCTED;	
		*this = obj;	//これが必須なのが不思議。何故かデフォルトコピーコンストラクタが仕事してくれない。
	}
	CEnemySpecies(){
		CONSTRUCTED;
		GoldGene = -1;
		ExpGene = -1;
		FirstRandomPlanSettingFlag = false;
			//RandomPlanSetの初期化はEnemySpeciesManagerのCreateで
	}
	~CEnemySpecies(){
		DESTRUCTED;	
	}

protected:
	CEnemyAI AI;

private:
	int GoldGene;
	int ExpGene;

	bool FirstRandomPlanSettingFlag;

};


////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////
