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
	
	if ((*RandomPlanSet).find(_randomPlan_key)==(*RandomPlanSet).end()){
		ErrorDx("Error->Planner:%s: Not Found RandomPlan :%d",__FILE__, __LINE__, EnemyName.c_str(), _randomPlan_key);
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

CEnemyPlanner_MYHP::CEnemyPlanner_MYHP(std::string _name, std::vector <std::string> _argList, std::map<int,std::vector<std::pair<int,int> > > *_randomPlanSet):CEnemyPlanner(_name, _randomPlanSet){
	CONSTRUCTED
	std::vector <int> numset;
	for (unsigned int i=0; i<_argList.size(); i++){
		int num;
		if(!( mystrtol(_argList[i].c_str(), &num))){
			ErrorDx("Error->CEnemyPlanner_MYHP(%s)->Check argument type[%d]", __FILE__, __LINE__, _name.c_str(), i);
			num = -1;
		}
		numset.push_back(num);
	}
	if (numset.size()%2==1) numset.push_back(0);	//引数が奇数の場合、最後尾に0%を追加

	for (unsigned int i=0; i<numset.size()/2; i++){
		PlanList.push_back( std::pair<int,int>(numset[i*2],numset[i*2+1]));
	}

	//ログ出力
	//for (unsigned int i=0; i<PlanList.size(); i++){
	//	myLog("%s:MYHP_AIList[%d]=(%d,%d)", _name.c_str(), i, PlanList[i].first, PlanList[i].second);
	//}
	//for (unsigned int i=0; i<_randomPlanSet->size(); i++){
	//	for (unsigned int j=0; j<(*_randomPlanSet)[i].size(); j++){
	//		myLog("%s:RandomPlan[%d][%d]=(%d,%d)", _name.c_str(),i,j,(*_randomPlanSet)[i][j].first, (*_randomPlanSet)[i][j].second);
	//	}
	//}
}
int CEnemyPlanner_MYHP::GetPlan(const CEnemy* _enemy){
	//ここに実際のあれこれを書く
	//PlanList･･･[0~n] first:TrickIndex, second:境界線となるHPパーセント

	int ratio = 100 * (double)_enemy->GetHp()/_enemy->GetMaxHp();
	int plan = -1;

	for (unsigned int i=0; i<PlanList.size(); i++){
		if (ratio > PlanList[i].second || i == PlanList.size()-1){
			plan = CalcRandomPlan(PlanList[i].first);
			return plan;
		}
	}

	return -1;
}

CEnemyPlanner_PLAYERNUM::CEnemyPlanner_PLAYERNUM(std::string _name, std::vector<std::string> _argList, std::map<int,std::vector<std::pair<int,int> > > *_randomPlanSet):CEnemyPlanner(_name, _randomPlanSet){
	CONSTRUCTED
	std::vector <int> numset;
	for (unsigned int i=0; i<_argList.size(); i++){
		int num;
		if(!( mystrtol(_argList[i].c_str(), &num))){
			ErrorDx("Error->CEnemyPlanner_PLAYERNUM(%s)->Check argument type[%d]", __FILE__, __LINE__, _name.c_str(), i);
			num = -1;
		}
		numset.push_back(num);
	}

	for (unsigned int i=0; i<MAX_PLAYER_NUM; i++){	//MAX_PLAYER_NUMよりもargList.size()が多いときは警告したほうがいいかも
		if (i < numset.size()){
			PlanList[i] = numset[i];
		}else{
			PlanList[i] = -1;
		}
	}
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
