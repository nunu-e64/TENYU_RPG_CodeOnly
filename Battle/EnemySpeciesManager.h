////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#ifndef ENEMYSPECIESMANAGER_H				////
#define ENEMYSPECIESMANAGER_H				////
////////////////////////////////////////////////

#include <map>
#include <string>
#include "Species.h"
#include "EnemyPlanner.h"


class CEnemySpeciesManager{
public:
	static CEnemySpeciesManager* GetInstance(){
		static CEnemySpeciesManager instance;
		return &instance;
	}
	~CEnemySpeciesManager(){
		DESTRUCTED;			
		Clear();
	}

	void Clear();

	bool CreateSpecies(const char* _name, int _maxhp, int _atk, int _def, int _spd, int _img);
	bool SetTrickList(const char* _name, std::vector <trick_tag const*> _trickList);
	//bool SetAI(const char* _name, CEnemyPlanner* _enemyPlanner);
	
	bool AddRandomPlanSet(const char* _name, unsigned int _index, std::vector<std::pair<int, int> > _planList, bool _defaultPlan=false);
	bool SetEnemyPlanner(std::string _enemyName, std::string _typeName, std::vector<std::string> _argList);
	bool SetEnemyTargetter(std::string _enemyName, std::string _typeName, std::vector<std::string> _argList);
	
	CEnemySpecies* GetSpecies(const char* _name);

	bool SetMapEncount(int _mapnum, int _chipnum, int _encount);
	bool AddMapEncountParty(int _mapnum, int _chipnum, int _encount, std::vector<std::string> _party);
	bool CheckEncount(int _mapnum, int _chipnum, std::vector<CEnemySpecies*> &_party_p);

	bool CheckAfterLoad();

private:
	bool EnemyBankLock;		//���ɔԁBvector�̗v�f�̃A�h���X���R�s�[�������Ƃ�vector�̗v�f��ω�������i���A�h���X���ς�邩������Ȃ��j���Ƃ̖����悤�ɊĎ�
	std::map <std::string, CEnemySpecies> EnemyBank;

	std::vector<const CEnemyPlanner* const> EnemyPlannerBank; 
	std::vector<const CEnemyTargetter* const> EnemyTargetterBank; 

	struct encount_tag{
		struct party_tag{
			int per;
			std::vector<CEnemySpecies*> party;
		};

		int encount;
		std::vector <party_tag> partyset;
	};
	std::map <int, std::map<int, encount_tag> > MapEncount;

	CEnemySpecies Dammy_Enemy;

	//�P�ꐫ��ۏ�(Singleton)//////////////////////////////////////////
		CEnemySpeciesManager(){
			CONSTRUCTED;
			EnemyBankLock = false;
		}
		CEnemySpeciesManager(const CEnemySpeciesManager& hoge);
		CEnemySpeciesManager& operator=(const CEnemySpeciesManager& hoge);
	//////////////////////////////////////////////////////////////////
};

////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										  //
////////////////////////////////////////////////