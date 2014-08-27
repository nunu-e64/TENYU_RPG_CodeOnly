#include "../Define.h"
#include "EnemyPlanManager.h"


void CEnemyPlanManager::Init(){
	EnemyPlanner.clear();

	for (int i=0; i<MAX_ENEMY; i++){
		CEnemyPlanner tmp;
		EnemyPlanner.push_back(tmp);
	}
}

void CEnemyPlanManager::MakePlan(int _enemyIndex, int _borderHp, int _choice1, int _choice2){

	char256 tmpkey;
	EnemyPlanner[_enemyIndex].PlanType = 0;

	mystrcpy(tmpkey.text, "BorderHp");
	EnemyPlanner[_enemyIndex].Value.insert(std::map<char256, int>::value_type( tmpkey, _borderHp));
	mystrcpy(tmpkey.text, "Choice1");
	EnemyPlanner[_enemyIndex].Value.insert(std::map<char256, int>::value_type( tmpkey, _choice1));
	mystrcpy(tmpkey.text, "Choice2");
	EnemyPlanner[_enemyIndex].Value.insert(std::map<char256, int>::value_type( tmpkey, _choice2));

}


int CEnemyPlanManager::Plan(CEnemy* _enemy){

	return 0;	//$$

}

int CEnemyPlanManager::Calc_HpBorder(float _hpBorder, int choice1, int choice2){



}


int CEnemyPlanManager::Calc_PlayerNum(int choice_1player, int choice_2player, int choice_3player){


}
