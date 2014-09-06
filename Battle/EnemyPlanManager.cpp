#include "../Define.h"
#include "EnemyPlanManager.h"
#include "Enemy.h"


void CEnemyPlanManager::Init(){
	EnemyPlanner.clear();

	for (int i=0; i<MAX_ENEMY; i++){
		CEnemyPlanner tmp;
		EnemyPlanner.push_back(tmp);
	}

	NowEnemy = NULL;
}

void CEnemyPlanManager::MakePlan(int _enemyIndex, int _borderHp, int _choice1, int _choice2){
	//�ŏI�I�ɂ͊O���t�@�C������̓ǂݍ��݉�
	EnemyPlanner[_enemyIndex].PlanType = 0;

	EnemyPlanner[_enemyIndex].Value.insert(std::map<std::string, int>::value_type( "BorderHp", _borderHp));
	EnemyPlanner[_enemyIndex].Value.insert(std::map<std::string, int>::value_type( "Choice1", _choice1));
	EnemyPlanner[_enemyIndex].Value.insert(std::map<std::string, int>::value_type( "Choice2", _choice2));

}


int CEnemyPlanManager::Plan(CEnemy* _enemy){
	CEnemyPlanner p =  EnemyPlanner[_enemy->GetIndex()];
	NowEnemy = _enemy;

	int choice = -1;

	switch(p.PlanType){
	case 0:
		choice = Calc_HpBorder(p.Value["BorderHp"]/100, p.Value["Choice1"], p.Value["Choice2"]);
		break;
	case 1:
		choice = Calc_PlayerNum(p.Value["Choice1"], p.Value["Choice2"], p.Value["Choice3"]); 
		break;
	default:
		break;
	}

	return choice;
}

int CEnemyPlanManager::Calc_HpBorder(float _hpBorder, int choice1, int choice2){
	if (NowEnemy->GetHp()>NowEnemy->GetMaxHp()*_hpBorder){
		return choice1;
	}else{
		return choice2;
	}	
}


int CEnemyPlanManager::Calc_PlayerNum(int choice_1player, int choice_2player, int choice_3player){
	int count=0;
	for (int i=0; i<MAX_PLAYER; i++){
		if (Actor[i]->GetAlive()) count++;
	}
	return choose(count, choice_1player, choice_2player, choice_3player);
}