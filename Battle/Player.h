////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#ifndef PLAYER_H							////
#define PLAYER_H							////
////////////////////////////////////////////////

#include "Actor.h"
#include "../Menu.h"


class CPlayer : public CActor{
public:
	CPlayer(){Mc=0;}
	void SetValue(int _atk, int _def, int _spd, int _maxhp);
	void Init();
	void Draw(int dx=0, int dy=0);

private:
	//�����o�֐�
		bool Plan();
		bool Action();

	//�����o�ϐ�
		int Mc;	//MagicCount?

		CBattleMenu BattleMenu;

};


////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										////
////////////////////////////////////////////////