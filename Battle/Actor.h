////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#ifndef ACTOR_H								////
#define ACTOR_H								////
////////////////////////////////////////////////

#include <vector>
#include <map>
#include <string>

#include "Species.h"

class CTextBox;		//�O���錾
class CCmdList;
class CBImgBank;

class CActor : public virtual CSpecies{
public:
	CActor(const CSpecies &obj) : CSpecies(obj){
		CONSTRUCTED
	}

	CActor(){
		CONSTRUCTED
	}
	~CActor(){
		DESTRUCTED
	}

	//�����ݒ�֘A
		void FirstSet(int _playernum, int _enemynum, int _index, CTextBox** _textbox, CCmdList* _cmdlist);
		void SetRect(int _cx, int _cy);
		//void SetImg(int _img); //�g���ĂȂ�
		bool SetSystemImg(CBImgBank* _bimgbank);
		void AddTrick(trick_tag const* _trick);
		//void ClearTrick(){TrickList.clear();} //�g���ĂȂ�

	bool Main();
	bool Do();
	virtual void Draw(int _dx=0, int _dy=0)=0;	//�������z�֐��c�e�N���X���ɒ�`�������K�v�������Ȃ�

	//��b���
		int GetIndex()const{return Index;}
		
	//�퓬�֘A
		void SetVisible(bool _visible){Visible = _visible;}
		bool GetVisible()const{return Visible;}
		bool GetAlive()const{return Alive;}
		//int GetDef(){return Def;}
		int GetAtk()const{return Atk;}
		int GetHp()const{return Hp;}
		int GetMaxHp()const{return MaxHp;}
		trick_tag const* GetNowTrick()const{return NowTrick;}

		int Damage(CActor* _attacker, trick_tag const* _trick);
		bool DeadCheck();	//���S�m�F���ς񂾂�True
		void SetTarget(int _target){Target=_target;}

	//���̑�
		CRect GetRect()const{return Rect;}

protected:
	//�����o�֐�
		virtual void Init(){}	//Player�̂ݕK�v�Ȃ̂ŁBEnemy�̎��ɂ͂�������ʂ�
		void Draw_Sub(int _dx=0, int _dy=0);

		virtual bool Plan()=0;
		virtual bool Action()=0;

		bool TimeGaugeForward();

		//void DamageStep(int _targetnum, ...);
		//void DamageStep(CActor* _target);


	//��b���
		int PLAYER_NUM;
		int ENEMY_NUM;
		int ActorIndex;	//Actor�S�̂̒ʂ��ԍ�
		int Index;	//Player,Enemy���Ƃ̒ʂ��ԍ�

	//�퓬�֘A
		bool Alive;
		bool Visible;
		//int Hp;
		int OldHp;	//�`��p
		double SpdPer;
		double TimeGauge;	//0~100%
		int Accident;	//��Ԉُ��X�e�[�^�X�ϓ�

		enum mode_tag{
			STAY,
			PLAN,
			PREPARE,
			ACTION,
			MODE_NUM
		}Mode;		//�ҋ@��r��,�s���I���Ȃ�

	//�Z�֘A
		trick_tag const* NowTrick;		//�uconst �� * �����O���ɂ���΁A�w��������̒l�������������Ȃ��B����ɂ���΁A�|�C���^�ϐ����̂������������Ȃ��v
		int Target;

	//�C���X�^���X�̃|�C���^�ێ��p
		CTextBox** B_TextBox_pp;
		CCmdList* CmdList;

	//�O���t�B�b�N
		CRect Rect;
		int Img_hpbar;
		int Img_timebar[2];
		int Dx; int Dy;
};


////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										////
////////////////////////////////////////////////