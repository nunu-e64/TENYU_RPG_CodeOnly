////多重インクルード防止（インクルードガード）//
#ifndef SPECIES_H							////
#define SPECIES_H							////
////////////////////////////////////////////////


class CSpecies{
public:
	/*CSpecies(const CSpecies& obj){
		*this = obj;
	}*/
	CSpecies(){
		Name="NULL_NAME";
		Lv=Img=Atk=Def=Spd=-1;
		MaxHp = 1;
	}
	~CSpecies(){}

	std::string GetName(){return Name;}

protected:
	std::string Name;
	int Img;

	int Lv;
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
	CEnemySpecies(){};

private:
	int GoldGene;
	int ExpGene;
};


////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////
