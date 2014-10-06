////多重インクルード防止（インクルードガード）//
#ifndef SPECIESMANAGER_H				////
#define SPECIESMANAGER_H				////
////////////////////////////////////////////////

#include <map>
#include <string>
#include "Species.h"


class CPlayerSpeciesManager{
public:
	CPlayerSpeciesManager(){
		Gold = 0;
	};
	void Clear(){Gold = 0; PlayerBank.clear(); MemberList.clear();}

	bool CreateSpecies(const char* _name, int _maxhp, int _atk, int _def, int _spd, int _img);
	bool SetTrickList(const char* _name, std::vector <trick_tag const*> _trickList);
	CPlayerSpecies GetSpecies(const char* _name);
	CPlayerSpecies GetSpecies(int _index);

	bool SetMemberList(int _index, const char* _name);
	bool SetMemberList();
	int GetMemberListSize(){return MemberList.size();}

private:
	std::map <std::string, CPlayerSpecies> PlayerBank;
	CPlayerSpecies Dammy_Player;

	int Gold;
	std::vector <CPlayerSpecies*> MemberList;		//プレイヤーリスト。0~MAX_PLAYER_NUM-1	が戦闘に出る

};


class CEnemySpeciesManager{
public:
	CEnemySpeciesManager(){};
	void Clear(){EnemyBank.clear();}

	bool CreateSpecies(const char* _name, int _maxhp, int _atk, int _def, int _spd, int _img);
	bool SetTrickList(const char* _name, std::vector <trick_tag const*> _trickList);

	CEnemySpecies GetSpecies(const char* _name);

private:
	std::map <std::string, CEnemySpecies> EnemyBank;

};


////多重インクルード防止（インクルードガード）//
#endif										  //
////////////////////////////////////////////////