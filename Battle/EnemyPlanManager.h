////多重インクルード防止（インクルードガード）//
#ifndef ENEMYPLANMANAGER_H					////
#define ENEMYPLANMANAGER_H					////
////////////////////////////////////////////////

#include "Actor.h"
#include <map>
#include <string>

class CEnemy;

class CEnemyPlanManager{
public:
	static CEnemyPlanManager* GetInstance(){
		static CEnemyPlanManager EnemyManager;
		return &EnemyManager;
	}
	void SetActor_p(CActor** _actorList){
		Actor = _actorList;
	}

	void Init(int _playernum, int _enemynum);
	void MakePlan(int _enemyIndex, int _borderHp, int _choice1, int _choice2);
	int Plan(CEnemy* _enemy);

private:
	class CEnemyPlanner{
	public:
		int PlanType;
		std::map <std::string, int> Value;
			//mapでは存在しないキーでアクセスしようとした場合、値をデフォルトコンストラクタで初期化し登録してくれやがる
	};
	std::vector <CEnemyPlanner> EnemyPlanner;	//mapでも可

	CEnemy* NowEnemy;

	//全アクターへのアクセスを持たせておく
		CActor** Actor;
		int PLAYER_NUM;
		int ENEMY_NUM;

	//具体的なAI構築用関数
		int Calc_HpBorder(float _hpBorder, int choice1, int choice2);
		int Calc_PlayerNum(int choice_1player, int choice_2player, int choice_3player);


	//単一性を保証(Singleton)
		CEnemyPlanManager(){}
		CEnemyPlanManager(const CEnemyPlanManager& hoge);
		CEnemyPlanManager& operator=(const CEnemyPlanManager& hoge);
};



////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////
