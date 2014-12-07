////多重インクルード防止（インクルードガード）//
#ifndef ENEMYPLANNER_H						////
#define ENEMYPLANNER_H						////
////////////////////////////////////////////////

class CActor;
class CEnemy;

class CEnemyPlanner{ //EnemySpeciesのAI用のインターフェース
public: 
	CEnemyPlanner(std::string _enemyname, std::map<int,std::vector<std::pair<int,int> > > *_randomPlanSet = NULL){
		EnemyName = _enemyname;
		RandomPlanSet = _randomPlanSet;
		PLAYER_NUM = 0;
		ENEMY_NUM = 0;
	}

	void BattleReady(const CActor* const* _actorList, const int _playerNum, const int _enemyNum){
		Actor = _actorList;
		PLAYER_NUM = _playerNum;
		ENEMY_NUM = _enemyNum;
	}

	std::string GetName() const{return EnemyName;}
	virtual int GetPlan(const CEnemy* _enemy)=0;
	int GetTarget(const CEnemy* _enemy){
		return rand()%PLAYER_NUM;
	}

protected:
	int CalcRandomPlan(int _randomPlan_key);

	//全アクターへのアクセスを持たせておく（戦闘開始ごとに更新）
		const CActor* const* Actor;
		int PLAYER_NUM;
		int ENEMY_NUM;

private:
	std::string EnemyName;
	std::map <int, std::vector<std::pair<int, int> > > *RandomPlanSet;
};

class CEnemyPlanner_DAMMY: public CEnemyPlanner{
public:
	CEnemyPlanner_DAMMY():CEnemyPlanner("Dammy_AI"){}

	int GetPlan(const CEnemy* _enemy){	//共通
		return -1;
	} 
};

class CEnemyPlanner_MYHP: public CEnemyPlanner{
public:
	CEnemyPlanner_MYHP(std::string _name, unsigned int _argnum, va_list args, std::map<int,std::vector<std::pair<int,int> > > *_randomPlanSet):CEnemyPlanner(_name, _randomPlanSet){
		int first;	int second;
		for (unsigned int i=0; i<_argnum/2; i++){
			first = va_arg(args,int);
			second = va_arg(args,int);
			PlanList.push_back( std::pair<int,int>(first, second) );
		}
		if (_argnum%2==1) PlanList.push_back( std::pair<int,int>(va_arg(args,int), 0) );

		for (unsigned int i=0; i<PlanList.size(); i++){
			myLog("%s:MYHP_AIList[%d]=(%d,%d)", _name.c_str(), i, PlanList[i].first, PlanList[i].second);
		}
		for (unsigned int i=0; i<_randomPlanSet->size(); i++){
			for (unsigned int j=0; j<(*_randomPlanSet)[i].size(); j++){
				myLog("%s:RandomPlan[%d][%d]=(%d,%d)", _name.c_str(),i,j,(*_randomPlanSet)[i][j].first, (*_randomPlanSet)[i][j].second);
			}
		}
	}
	int GetPlan(const CEnemy* _enemy); //共通
private:
	std::vector <std::pair<int, int> > PlanList;
};


class CEnemyPlanner_PLAYERNUM: public CEnemyPlanner{
public:
	CEnemyPlanner_PLAYERNUM(std::string _name, va_list args, std::map<int,std::vector<std::pair<int,int> > > *_randomPlanSet):CEnemyPlanner(_name, _randomPlanSet){
		for (int i=0; i<MAX_PLAYER_NUM; i++){
			PlanList[i] = va_arg(args, int);
		}
	}
	int GetPlan(const CEnemy* _enemy); //共通
private:
	int PlanList[MAX_PLAYER_NUM];
};


////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////