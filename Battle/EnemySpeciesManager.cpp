#include "../Define.h"
#include "EnemySpeciesManager.h"


CEnemySpeciesManager::CEnemySpeciesManager(void){

}


bool CEnemySpeciesManager::CreateEnemySpecies(const char* _name, int _maxhp, int _atk, int _def, int _spd){

	CEnemySpecies newEnemy;
	newEnemy.SetValue(_name, _maxhp, _atk, _def, _spd);
		
	EnemyBank.insert( std::map<std::string, CEnemySpecies>::value_type( _name, newEnemy) );
	
	return true;
}

bool CEnemySpeciesManager::SetTrickList(){

	return true;

};

CEnemySpecies* CEnemySpeciesManager::GetEnemySpecies(const char* _name){

	CEnemySpecies* tmp = &EnemyBank[_name];

	if (tmp->GetName().compare(_name)){
		return tmp;
	}else{
		ErrorDx("EnemySpeciesManager->GetEnemySpecies->NotFound:%s", __FILE__, __LINE__, _name);
		return NULL;
	}

}




