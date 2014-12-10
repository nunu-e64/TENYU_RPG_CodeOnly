////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#ifndef PLAYER_H							////
#define PLAYER_H							////
////////////////////////////////////////////////

#include "Actor.h"
#include "../Menu.h"


class CPlayer : public CActor, public CPlayerSpecies{
public:
	CPlayer(){CONSTRUCTED; Mc=0;}
	CPlayer(const CPlayerSpecies& obj) : CPlayerSpecies(obj){CONSTRUCTED; Mc=0;};
	~CPlayer(){DESTRUCTED;}
	
	void CreateBattleMenu();
	void Draw(int _dx=0, int _dy=0);

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