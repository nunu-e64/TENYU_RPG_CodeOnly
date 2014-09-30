#include "../Define.h"
#include "SpeciesManager.h"


bool CPlayerSpeciesManager::CreateSpecies(const char* _name, int _maxhp, int _atk, int _def, int _spd){
	CPlayerSpecies newPlayer;
	newPlayer.SetValue(_name, _maxhp, _atk, _def, _spd);
	PlayerBank.insert( std::map<std::string, CPlayerSpecies>::value_type( _name, newPlayer) );
	return true;
}

bool CPlayerSpeciesManager::SetTrickList(const char* _name, std::vector <trick_tag const*> _trickList){
	PlayerBank[_name].TrickList = _trickList;
	return true;
}

bool CPlayerSpeciesManager::SetImg(const char* _name, int _img){
	PlayerBank[_name].Img = _img;
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
bool CEnemySpeciesManager::CreateSpecies(const char* _name, int _maxhp, int _atk, int _def, int _spd){
	CEnemySpecies newEnemy;
	newEnemy.SetValue(_name, _maxhp, _atk, _def, _spd);
	EnemyBank.insert( std::map<std::string, CEnemySpecies>::value_type( _name, newEnemy) );
	return true;
}

bool CEnemySpeciesManager::SetTrickList(const char* _name, std::vector <trick_tag const*> _trickList){
	EnemyBank[_name].TrickList = _trickList;
	return true;
}

bool CEnemySpeciesManager::SetImg(const char* _name, int _img){
	EnemyBank[_name].Img = _img;
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
