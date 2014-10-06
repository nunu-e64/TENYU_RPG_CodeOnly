////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#ifndef SPECIES_H							////
#define SPECIES_H							////
////////////////////////////////////////////////


class CSpecies{
public:
	/*CSpecies(const CSpecies& obj){
		*this = obj;
	}*/
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
	int Spd;		//%�����Βl
	
	std::vector <trick_tag const*> TrickList;		//�Z���X�g

	void SetValue(const char* _name, int _maxhp, int _atk, int _def, int _spd);

};


class CPlayerSpecies : public virtual CSpecies{
	friend class CPlayerSpeciesManager;

public:
	CPlayerSpecies(const CPlayerSpecies& obj){
		*this = obj;	//���ꂪ�K�{�Ȃ̂��s�v�c�B���̂��f�t�H���g�R�s�[�R���X�g���N�^���d�����Ă���Ȃ��B
	}
	CPlayerSpecies(){};
	~CPlayerSpecies(){};
};

class CEnemySpecies : public virtual CSpecies{
	friend class CEnemySpeciesManager;

public:
	CEnemySpecies(const CEnemySpecies& obj){
		*this = obj;	//���ꂪ�K�{�Ȃ̂��s�v�c�B���̂��f�t�H���g�R�s�[�R���X�g���N�^���d�����Ă���Ȃ��B
	}
	CEnemySpecies(){};
	~CEnemySpecies(){};
};


////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										////
////////////////////////////////////////////////
