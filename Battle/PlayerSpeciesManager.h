////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#ifndef PLAYERSPECIESMANAGER_H				////
#define PLAYERSPECIESMANAGER_H				////
////////////////////////////////////////////////

#include <map>
#include <string>
#include "Species.h"

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

	bool CPlayerSpeciesManager::CheckAfterLoad();

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
////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										  //
////////////////////////////////////////////////