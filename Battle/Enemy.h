////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#ifndef ENEMY_H								////
#define ENEMY_H								////
////////////////////////////////////////////////

#include "Actor.h"

class CEnemy;
class CEnemyPlanner{
public:
	//void MakePlan();
	int Plan(CEnemy* _enemy);
};


class CEnemy : public CActor{	
	friend class CEnemyPlanner;
public:
	void Draw(int dx=0, int dy=0);

private:
	//�����o�֐�
		bool Plan();
		bool Action();

	//�����o�ϐ�
		CEnemyPlanner EnemyPlanner;
		int Attention[MAX_PLAYER];
};



////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										////
////////////////////////////////////////////////
