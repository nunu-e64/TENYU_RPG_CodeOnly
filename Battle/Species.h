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
	int Spd;		//%→相対値
	
	std::vector <trick_tag const*> TrickList;		//技リスト
	
	//＠コマンドの順番管理
		bool TrickSet;
		bool FirstRandomPlanSet;
		bool AISet;
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

	std::map <int, std::vector<std::pair<int, int> > > RandomPlanSet;
		//RandomPlanSet[index] = (choice, probability)
		//行動選択肢とその発動比を並べたリスト。
		//行動計算の為にAI.PlannerとAI.Targetにポインタを渡しておく

};


////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////
