////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#ifndef SPECIESMANAGER_H				////
#define SPECIESMANAGER_H				////
////////////////////////////////////////////////

#include <map>
#include <string>
#include "Species.h"
#include "EnemyPlanner.h"

class CPlayer;

class CPlayerSpeciesManager{
public:
	static CPlayerSpeciesManager* GetInstance(){
		static CPlayerSpeciesManager instance;
		return &instance;
	}
	~CPlayerSpeciesManager(){
		DESTRUCTED;		
	}

	void Clear();

	bool CreateSpecies(const char* _name, int _maxhp, int _atk, int _def, int _spd, int _img);
	bool SetTrickList(const char* _name, std::vector <trick_tag const*> _trickList);
	CPlayerSpecies* GetSpecies(const char* _name);
	CPlayerSpecies* GetSpecies(int _index);

	bool SetMemberList(int _index, const char* _name);
	bool SetMemberList();
	int GetMemberListSize(){return MemberList.size();}

	void AddGold(int _gold){Gold+=_gold;}
	void CopyValue(int PLAYER_NUM, CPlayer* _player); 

private:
	bool PlayerBankLock;
	std::map <std::string, CPlayerSpecies> PlayerBank;
	CPlayerSpecies Dammy_Player;

	int Gold;	//������
	std::vector <CPlayerSpecies*> MemberList;		//�v���C���[���X�g�B0~MAX_PLAYER_NUM-1	���퓬�ɏo��

	//�P�ꐫ��ۏ�(Singleton)//////////////////////////////////////////
		CPlayerSpeciesManager(){
			CONSTRUCTED;
			PlayerBankLock = false;
			Gold = 0;
		}
		CPlayerSpeciesManager(const CPlayerSpeciesManager& hoge);
		CPlayerSpeciesManager& operator=(const CPlayerSpeciesManager& hoge);
	//////////////////////////////////////////////////////////////////

};


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
	
	bool SetRandomPlanSet(const char* _name, unsigned int _index, std::vector<std::pair<int, int> > _planList);
	bool SetEnemyPlanner(std::string _enemyName, std::string _typeName, std::vector<std::string> _argList);

	CEnemySpecies* GetSpecies(const char* _name);

	bool SetMapEncount(int _mapnum, int _chipnum, int _encount);
	bool AddMapEncountParty(int _mapnum, int _chipnum, int _encount, std::vector<std::string> _party);
	bool CheckEncount(int _mapnum, int _chipnum, std::vector<CEnemySpecies*> &_party_p);

private:
	bool EnemyBankLock;		//���ɔԁBvector�̗v�f�̃A�h���X���R�s�[�������Ƃ�vector�̗v�f��ω�������i���A�h���X���ς�邩������Ȃ��j���Ƃ̖����悤�ɊĎ�
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
	CEnemyPlanner_DAMMY Dammy_AI;

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