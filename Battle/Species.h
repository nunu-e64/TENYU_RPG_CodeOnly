////多重インクルード防止（インクルードガード）//
#ifndef SPECIES_H							////
#define SPECIES_H							////
////////////////////////////////////////////////


class CSpecies{
public:
	CSpecies(){
		Name="";
		Img=Atk=Def=Spd=-1;
		MaxHp = 1;
	}
	~CSpecies(){}

	std::string GetName(){return Name;}

protected:
	std::string Name;
	int Img;

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
	CPlayerSpecies(){};
	~CPlayerSpecies(){};
};

class CEnemySpecies : public virtual CSpecies{
	friend class CEnemySpeciesManager;

public:
	CEnemySpecies(){};
	~CEnemySpecies(){};
};


////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////
