////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#ifndef ENEMY_H								////
#define ENEMY_H								////
////////////////////////////////////////////////

#include "Actor.h"

class CEnemyPlanManager;

class CEnemy : public CActor{	
public:
	
	void SetEnemyPlanManager(CEnemyPlanManager* _enemyPlanManager){
		EnemyPlanManager = _enemyPlanManager;
	}

	void Draw(int dx=0, int dy=0);
	void MakePlan();	
	
private:
	//�����o�֐�
		bool Plan();
		bool Action();

	//�����o�ϐ�
		CEnemyPlanManager* EnemyPlanManager;
		int Attention[MAX_PLAYER];
};



////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										////
////////////////////////////////////////////////
