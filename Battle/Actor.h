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
class CLogWindow;

class CActor : public virtual CSpecies{
public:
	CActor(const CSpecies &obj) : CSpecies(obj){
		CONSTRUCTED;
	}

	CActor(){
		CONSTRUCTED;
	}
	~CActor(){
		DESTRUCTED;
	}

	//�����ݒ�֘A
		void FirstSet(int _playernum, int _enemynum, int _index, CTextBox** _textbox, CCmdList* _cmdlist, CLogWindow* _logWindow);
		void SetRect(int _cx, int _cy);
		//void SetImg(int _img); //�g���ĂȂ�
		bool SetSystemImg(CBImgBank* _bimgbank);
		//void AddTrick(trick_tag const* _trick);	//�g���ĂȂ�
		//void ClearTrick(){TrickList.clear();} //�g���ĂȂ�

	bool Main();
	bool Do();
	virtual void Draw(int _dx=0, int _dy=0)=0;	//�������z�֐��c�e�N���X���ɒ�`�������K�v�������Ȃ�

	//��b���
		int GetIndex()const{return Index;}
		bool IsPlayer()const{
			return (ActorIndex<PLAYER_NUM);
		}
		
	//�퓬�֘A
		bool GetAlive()const{ return Alive; }
		int GetVisibleStatus()const{ return VisibleStatus;}
		bool GetVisible()const{ return Visible; }
		void SetVisible(bool _visible){ Visible = _visible;}
		
		//int GetDef(){return Def;}
		int GetAtk()const{ return Atk; }	//�\���̍����GetValue�Ƃ��ł܂Ƃ߂�������������
		int GetHp()const{ return Hp; }
		int GetMaxHp()const{ return MaxHp; }
		trick_tag const* GetNowTrick()const{ return NowTrick; }

		bool GetStatus(int _key);

		int Damaged(CActor* _attacker, trick_tag const* _trick);
		bool CheckBarMove();	//���S�m�F���ς񂾂�True
		void SetTarget(int _target){ Target=_target; }

		void ChangeValue(int _kind, int _powerPercent);

	//���̑�
		CRect GetRect()const{return Rect;}

protected:
	//�����o�֐�
		virtual void SetExtraImg(CBImgBank* _bImgBank){}	//SetSystemImg����Ăяo���BPlayer�ɂ̂ݕK�v�ł��邽��Enemy�̎��ɂ͂�������ʂ�
		
		void Draw_Sub(int _dx=0, int _dy=0);

		virtual bool Plan()=0;
		virtual bool Action()=0;

	//��b���
		int PLAYER_NUM;
		int ENEMY_NUM;
		int ActorIndex;	//Actor�S�̂̒ʂ��ԍ�
		int Index;	//Player,Enemy���Ƃ̒ʂ��ԍ�

	//�퓬�֘A
		bool Alive;		//�����B���񂾂瑦���ɕύX
		bool Visible;	//�P���ɕ`�悳��邩�ۂ��B�����͌p���B�_�ŕ\���Ɏg�p�B
		enum{
			VISIBLE,
			CHANGING,
			INVISIBLE
		}VisibleStatus;	//�������ł̕`����e�ύX���Ǘ�
		int OldHp;	//�`��p
		double SpdPer;
		//int Accident;	//��Ԉُ��X�e�[�^�X�ϓ� ���g�p

		std::map <int, bool> Status;

		enum mode_tag{
			STAY,
			PLAN,
			PREPARE,
			ACTION,
			MODE_NUM
		}Mode;		//�ҋ@��r��,�s���I���Ȃ�	//private�ɂ�������Player�̖h�䏈���ŃA�N�Z�X���Ă�(15/04/26)
		int MaxTimeGauge; //0~XXX(%)

	//�Z�֘A
		trick_tag const* NowTrick;		//�uconst �� * �����O���ɂ���΁A�w��������̒l�������������Ȃ��B����ɂ���΁A�|�C���^�ϐ����̂������������Ȃ��v
		int Target;

	//�C���X�^���X�̃|�C���^�ێ��p
		CTextBox** B_TextBox_pp;
		CCmdList* CmdList;
		CLogWindow* LogWindow;

	//�O���t�B�b�N
		CRect Rect;
		int Img_hpbar;
		int Img_timebar[2];
		int Dx; int Dy;

private:
		bool TimeGaugeForward();	//�����I�ɂ͌���
		double TimeGauge;	//MaxTimeGauge��0(%)
		
		virtual double CalcDamage(double _damage, CActor* _attacker, trick_tag const* _trick){
			return _damage;
		};
};


////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										////
////////////////////////////////////////////////