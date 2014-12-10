////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
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
	int Spd;		//%�����Βl
	
	std::vector <trick_tag const*> TrickList;		//�Z���X�g

	void SetValue(const char* _name, int _maxhp, int _atk, int _def, int _spd);

};


class CPlayerSpecies : public virtual CSpecies{
	friend class CPlayerSpeciesManager;
public:
	CPlayerSpecies(const CPlayerSpecies& obj){
		CONSTRUCTED;
		*this = obj;	//���ꂪ�K�{�Ȃ̂��s�v�c�B���̂��f�t�H���g�R�s�[�R���X�g���N�^���d�����Ă���Ȃ��B
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
		*this = obj;	//���ꂪ�K�{�Ȃ̂��s�v�c�B���̂��f�t�H���g�R�s�[�R���X�g���N�^���d�����Ă���Ȃ��B
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


////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										////
////////////////////////////////////////////////
