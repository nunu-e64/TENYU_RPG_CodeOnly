////多重インクルード防止（インクルードガード）//
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
	
	bool AddRandomPlanSet(const char* _name, unsigned int _index, std::vector<std::pair<int, int> > _planList);
	bool SetEnemyPlanner(std::string _enemyName, std::string _typeName, std::vector<std::string> _argList);

	CEnemySpecies* GetSpecies(const char* _name);

	bool SetMapEncount(int _mapnum, int _chipnum, int _encount);
	bool AddMapEncountParty(int _mapnum, int _chipnum, int _encount, std::vector<std::string> _party);
	bool CheckEncount(int _mapnum, int _chipnum, std::vector<CEnemySpecies*> &_party_p);

private:
	bool EnemyBankLock;		//金庫番。vectorの要素のアドレスをコピーしたあとにvectorの要素を変化させる（＝アドレスが変わるかもしれない）ことの無いように監視
	std::map <std::string, CEnemySpecies> EnemyBank;
	std::vector<const CEnemyPlanner* const> EnemyPlannerBank; 
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
	CEnemyPlanner_DAMMY Dammy_Planner;

	//単一性を保証(Singleton)//////////////////////////////////////////
		CEnemySpeciesManager(){
			CONSTRUCTED;
			EnemyBankLock = false;
		}
		CEnemySpeciesManager(const CEnemySpeciesManager& hoge);
		CEnemySpeciesManager& operator=(const CEnemySpeciesManager& hoge);
	//////////////////////////////////////////////////////////////////
};

////多重インクルード防止（インクルードガード）//
#endif										  //
////////////////////////////////////////////////