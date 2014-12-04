////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#ifndef ENEMY_H								////
#define ENEMY_H								////
////////////////////////////////////////////////

#include "Actor.h"

class CEnemyPlanManager;
class CEnemySpecies;

class CEnemy : public CActor, public CEnemySpecies{
public:
	
	CEnemy(){};
	CEnemy(const CEnemySpecies& obj) : CEnemySpecies(obj){};

	void SetEnemyPlanManager(CEnemyPlanManager* _enemyPlanManager){
		EnemyPlanManager = _enemyPlanManager;
	}

	void Draw(int _dx=0, int _dy=0);
	void MakePlan();

	//CEnemyPlannner AI;
	
private:
	//�����o�֐�
		bool Plan();
		bool Action();

	//�����o�ϐ�
		CEnemyPlanManager* EnemyPlanManager;
		int Attention[MAX_PLAYER_NUM];
};



////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										////
////////////////////////////////////////////////
