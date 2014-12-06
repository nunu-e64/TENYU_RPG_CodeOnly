#include "../Define.h"
#include "EnemyPlanner.h"
#include "Enemy.h"


int CEnemyPlanner::CalcRandomPlan(int _randomPlan_key){
	int plan = -1;

	if (typeid(*this) == typeid(CEnemyPlanner_DAMMY)){
		WarningDx("Warning->CEnemyPlanner(%s)::CalcRandomPlan->DammyPlanner!", __FILE__, __LINE__, EnemyName.c_str());
		return -1;
	}else if(RandomPlanSet==NULL){
		WarningDx("Error->CEnemyPlanner(%s)::CalcRandomPlan->RandomPlanSet == NULL!", __FILE__, __LINE__, EnemyName.c_str());
		return -1;
	}	
	
	if (RandomPlanSet->find(_randomPlan_key)==RandomPlanSet->end()){
		ErrorDx("Error->Not Found RandomPlan :%d\nPlanner:%s",__FILE__, __LINE__, _randomPlan_key, EnemyName.c_str());
		plan =  -1;

	}else{
		int probability=0;

		for (unsigned int i=0; i<(*RandomPlanSet)[_randomPlan_key].size(); i++){
			probability += (*RandomPlanSet)[_randomPlan_key][i].second;
		    if ((rand()%1000)/(double)1000 * probability < (*RandomPlanSet)[_randomPlan_key][i].second){
				plan = (*RandomPlanSet)[_randomPlan_key][i].first;
			}
		}
	}

	return plan;
}

int CEnemyPlanner_MYHP::GetPlan(const CEnemy* _enemy){
	//ここに実際のあれこれを書く
	//PlanList･･･[0~n] first:TrickIndex, second:境界線となるHPパーセント

	int ratio = _enemy->GetHp()/_enemy->GetMaxHp();
	int plan = -1;

	for (unsigned int i=0; i<PlanList.size(); i++){
		if (ratio > PlanList[i].second || i == PlanList.size()-1){
			plan = CalcRandomPlan(PlanList[i].first);
			return plan;
		}
	}

	return -1;
}


int CEnemyPlanner_PLAYERNUM::GetPlan(const CEnemy* _enemy){
	//ここに実際のあれこれを書く

	int alivePlayerNum = 0;
	for (int i=0; i<PLAYER_NUM; i++){
		if (Actor[i]->GetAlive()) ++alivePlayerNum;
	}

	int plan = -1;
	if(alivePlayerNum == 0){
		plan = -1;
	}else{
		plan = CalcRandomPlan(PlanList[alivePlayerNum-1]);
	}

	return plan;
}
