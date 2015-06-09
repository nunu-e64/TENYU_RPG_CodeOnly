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
		
		Level = -1;
		Hp = MaxHpGene = -1;
		AtkGene = DefGene = SpdGene = -1;

		TrickList.clear();
	}
	~CSpecies(){DESTRUCTED;}

	std::string GetName()const{return Name;}

protected:

	void SetValue(const char* _name, int _level, int _geneMaxHp, int _geneAtk, int _geneDef, int _geneSpd);
	//int CalcValue(value_tag _key, int _level, int _gene);

	std::string Name;
	int Img;	//UNDONE:画像類はいずれPlayerSpeciesとEnemySpeciesで別に管理する

	int Level;
	int Hp;

	int MaxHpGene;
	int AtkGene;
	int DefGene;
	int SpdGene;
		
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
		Exp = 0;
		MagicCount = MAX_MAGIC_COUNT;
		BaseTrickPowerGene = -1;
	}
	~CPlayerSpecies(){
		DESTRUCTED;
	}

protected:
	int MagicCount;	//魔力。上限はMAX_MAGIC_COUNT。
	trick_tag BaseTrick; 

	int BaseTrickPowerGene;

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

	std::vector <std::pair<std::string, int> > DropItemList;	//first:ItemName, second:DropProbability

private:
	int GoldGene;
	int ExpGene;

	bool FirstRandomPlanSettingFlag;
};


////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////
