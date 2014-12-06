////多重インクルード防止（インクルードガード）//
#ifndef SPECIES_H							////
#define SPECIES_H							////
////////////////////////////////////////////////

#include "EnemyPlanner.h"

class CSpecies{
public:
	/*CSpecies(const CSpecies& obj){
		*this = obj;
	}*/
	CSpecies(){
		Name="NULL_NAME";
		Lv=Img=Atk=Def=Spd=-1;
		Hp=MaxHp = 1;
	}
	~CSpecies(){}

	std::string GetName(){return Name;}

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
		*this = obj;	//これが必須なのが不思議。何故かデフォルトコピーコンストラクタが仕事してくれない。
	}
	CPlayerSpecies(){Exp=0;};

private:
	int Exp;
};


class CEnemySpecies : public virtual CSpecies{
	friend class CEnemySpeciesManager;
public:
	CEnemySpecies(const CEnemySpecies& obj){
		*this = obj;	//これが必須なのが不思議。何故かデフォルトコピーコンストラクタが仕事してくれない。
	}
	CEnemySpecies(){
		AI = NULL;
	}
	~CEnemySpecies(){
		myLog("%s:%s:Destructed0", typeid(*this).name(), Name.c_str());
		if (AI!=NULL && AI->GetName()==Name) {
			myLog("Destructed2");
			delete AI;
			myLog("Destructed3");
		}
		myLog("Destructed_end");
	}

protected:
	CEnemyPlanner* AI;

private:
	int GoldGene;
	int ExpGene;
	std::map <int, std::vector<std::pair<int, int> > > RandomPlanSet;


};


////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////
