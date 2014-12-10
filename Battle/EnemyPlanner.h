////多重インクルード防止（インクルードガード）//
#ifndef ENEMYPLANNER_H						////
#define ENEMYPLANNER_H						////
////////////////////////////////////////////////

class CActor;
class CEnemy;

class CEnemyPlanner{ //EnemySpeciesのAI用のインターフェース
public: 
	CEnemyPlanner(std::string _enemyname, std::map<int,std::vector<std::pair<int,int> > > *_randomPlanSet = NULL){
		CONSTRUCTED;
		EnemyName = _enemyname;
		RandomPlanSet = _randomPlanSet;
		PLAYER_NUM = 0;
		ENEMY_NUM = 0;
	}

	~CEnemyPlanner(){
		DESTRUCTED;	
	}

	void BattleReady(const CActor* const* _actorList, const int _playerNum, const int _enemyNum){
		Actor = _actorList;
		PLAYER_NUM = _playerNum;
		ENEMY_NUM = _enemyNum;
	}

	std::string GetName() const{return EnemyName;}
	virtual int GetPlan(const CEnemy* _enemy)=0;

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
	CEnemyPlanner_DAMMY():CEnemyPlanner("Dammy_AI"){CONSTRUCTED;}
	~CEnemyPlanner_DAMMY(){DESTRUCTED;}

	int GetPlan(const CEnemy* _enemy){	//共通
		return -1;
	} 
};

class CEnemyPlanner_MYHP: public CEnemyPlanner{
public:
	CEnemyPlanner_MYHP(std::string _name, std::vector<std::string> _argList, std::map<int,std::vector<std::pair<int,int> > > *_randomPlanSet); //:CEnemyPlanner(_name, _randomPlanSet);
	~CEnemyPlanner_MYHP(){DESTRUCTED;}
	int GetPlan(const CEnemy* _enemy); //共通
private:
	std::vector <std::pair<int, int> > PlanList;
};


class CEnemyPlanner_PLAYERNUM: public CEnemyPlanner{
public:
	CEnemyPlanner_PLAYERNUM(std::string _name, std::vector<std::string> _argList, std::map<int,std::vector<std::pair<int,int> > > *_randomPlanSet);
	~CEnemyPlanner_PLAYERNUM(){DESTRUCTED;}
	int GetPlan(const CEnemy* _enemy); //共通
private:
	int PlanList[MAX_PLAYER_NUM];
};


////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////