////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#ifndef ENEMYPLANMANAGER_H					////
#define ENEMYPLANMANAGER_H					////
////////////////////////////////////////////////

#include "EnemyPlanner.h"
#include "Actor.h"

class CEnemy;

class CEnemyPlanManager{
public:
	static CEnemyPlanManager* GetInstance(){
		static CEnemyPlanManager EnemyManager;
		return &EnemyManager;
	}
	~CEnemyPlanManager(){
		myLog("MyClassLog", "%s.Denstructed", typeid(*this).name());
	}
	
	int GetTarget(CEnemy* _enemy);

private:

	//std::vector <const CEnemyPlanner*> EnemyPlannerBank;

	//�P�ꐫ��ۏ�(Singleton)//////////////////////////////////////////
		CEnemyPlanManager(){
			CONSTRUCTED
		}
		CEnemyPlanManager(const CEnemyPlanManager& hoge);
		CEnemyPlanManager& operator=(const CEnemyPlanManager& hoge);
	///////////////////////////////////////////////////////////////////
};


////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										////
////////////////////////////////////////////////
