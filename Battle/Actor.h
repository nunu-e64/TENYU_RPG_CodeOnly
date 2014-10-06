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
	CActor(const CSpecies &obj) : CSpecies(obj){}

	CActor(){}	//Actor = new CActor* [MAX_PLAYER_NUM+MAX_ENEMY_NUM];}
	~CActor(){}	//delete [] Actor;}

	//�����ݒ�֘A
		void FirstSet(int _index, CTextBox** _textbox, CCmdList* _cmdlist);
		void SetRect(int _cx, int _cy);
		void SetImg(int _img);
		bool SetSystemImg(CBImgBank* _bimgbank);
		void AddTrick(trick_tag const* _trick);
		void ClearTrick(){TrickList.clear();}

	bool Main();
	bool Do();
	virtual void Draw(int dx=0, int dy=0)=0;	//�������z�֐��c�e�N���X���ɒ�`�������K�v�������Ȃ�

	//��b���
		int GetIndex(){return Index;}
		
	//�퓬�֘A
		bool GetAlive(){return Alive;}
		//int GetDef(){return Def;}
		int GetAtk(){return Atk;}
		int GetHp(){return Hp;}
		int GetMaxHp(){return MaxHp;}
		trick_tag const* GetNowTrick(){return NowTrick;}

		int Damage(CActor* _attacker, trick_tag const* _trick);
		bool DeadCheck();	//���S�m�F���ς񂾂�True
		void SetTarget(int _target){Target=_target;}

	//�ق�
		CRect GetRect(){return Rect;}

protected:
	//�����o�֐�
		virtual void Init(){}	//Player�̂ݕK�v�Ȃ̂ŁBEnemy�̎��ɂ͂�������ʂ�
		void Draw_Sub(int dx=0, int dy=0);

		virtual bool Plan()=0;
		virtual bool Action()=0;

		bool TimeGaugeForward();

		//void DamageStep(int _targetnum, ...);
		//void DamageStep(CActor* _target);


	//��b���
		int ActorIndex;	//Actor�S�̂̒ʂ��ԍ�
		int Index;	//Player,Enemy���Ƃ̒ʂ��ԍ�

	//�퓬�֘A
		bool Alive;
		bool Visible;
		int Hp;		int OldHp;	//�`��p
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

	//��܃N���X
		CTextBox** B_TextBox_pp;
		CCmdList* CmdList;

	//�O���t�B�b�N
		CRect Rect;
		int Img_hpbar;
		int Img_timebar[2];
		//...
};


////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										////
////////////////////////////////////////////////