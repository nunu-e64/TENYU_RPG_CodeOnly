#include "../Define.h"
#include "SpeciesManager.h"


bool CPlayerSpeciesManager::CreateSpecies(const char* _name, int _maxhp, int _atk, int _def, int _spd, int _img){
	CPlayerSpecies newPlayer;
	newPlayer.SetValue(_name, _maxhp, _atk, _def, _spd);
	newPlayer.Img = _img;
	PlayerBank.insert( std::map<std::string, CPlayerSpecies>::value_type( _name, newPlayer) );

	return true;
}

bool CPlayerSpeciesManager::SetTrickList(const char* _name, std::vector <trick_tag const*> _trickList){
	PlayerBank[_name].TrickList = _trickList;
	return true;
}

bool CPlayerSpeciesManager::SetBattleMember(int _index, const char* _name){
	if (_index<0){
		ErrorDx("Error->SetBattleMember->size error:%d", _index);
		return false;
	}else if (_index>=(int)(BattleMember.size())){
		BattleMember.push_back(_name);
	}else{
		BattleMember[_index] = _name;
	}
	return true;
}
bool CPlayerSpeciesManager::SetBattleMember(int _num){
	std::map<std::string, CPlayerSpecies>::iterator it=PlayerBank.begin();
	BattleMember.clear();
	int i = 0;
	while( it != PlayerBank.end() && i<_num){
		BattleMember.push_back((*it).second.GetName());
		++it;
		++i;
	}
	return true;
}

CPlayerSpecies CPlayerSpeciesManager::GetSpecies(const char* _name){
	CPlayerSpecies* tmp = &PlayerBank[_name];

	if (tmp->GetName()==_name){
		return *tmp;
	}else{
		ErrorDx("PlayerSpeciesManager->GetPlayerSpecies->NotFound:%s", __FILE__, __LINE__, _name);
		return CPlayerSpecies();
	}
}

//////////////////////////////////////////////////////////////////////////////
bool CEnemySpeciesManager::CreateSpecies(const char* _name, int _maxhp, int _atk, int _def, int _spd, int _img){
	CEnemySpecies newEnemy;
	newEnemy.SetValue(_name, _maxhp, _atk, _def, _spd);
	newEnemy.Img = _img;
	EnemyBank.insert( std::map<std::string, CEnemySpecies>::value_type( _name, newEnemy) );
	return true;
}

bool CEnemySpeciesManager::SetTrickList(const char* _name, std::vector <trick_tag const*> _trickList){
	EnemyBank[_name].TrickList = _trickList;
	return true;
}

CEnemySpecies CEnemySpeciesManager::GetSpecies(const char* _name){
	CEnemySpecies* tmp = &EnemyBank[_name];

	if (tmp->GetName()==_name){
		return *tmp;
	}else{
		ErrorDx("EnemySpeciesManager->GetEnemySpecies->NotFound:%s", __FILE__, __LINE__, _name);
		return CEnemySpecies();
	}
}
