////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#ifndef TRICKDAMAGEEFFECT_H					////
#define TRICKDAMAGEEFFECT_H					////
////////////////////////////////////////////////

#include <map>
#include <string>
//#include "Battle.h"

class CBattle;
class CBImgBank;
class CTrickDamageEffect{ //�퓬�_���[�W�G�t�F�N�g�p�̃C���^�[�t�F�[�X
public: 
	CTrickDamageEffect(std::string _name){
		Name = _name;
	}
	virtual void DrawDamageEffect(CBattle* _battle, CBImgBank* _bimgbank, CRect _attackerR, CRect _targetR) const =0;
	std::string GetName() const {return Name;}
protected:
	std::string Name;	//�G�t�F�N�g�̖��O�@��FRED_BOMB, BLUE_BOMB, BIG_SPREAD,�@SMALL_SPREAD
	//std::string TypeName;	//Type�̖��O�@��FBOMB, SPREAD
};


class CTrickDamageEffect_BOMB: public CTrickDamageEffect{
public:
	CTrickDamageEffect_BOMB(std::string _name, va_list args):CTrickDamageEffect(_name){
		Color = va_arg(args, int);
		Size = va_arg(args, int);
		Time = va_arg(args, int);
	}
	void DrawDamageEffect(CBattle* _battle, CBImgBank* _bimgbank, CRect _attackerR, CRect _targetR) const;	//����
private:
	int Color;
	int Size;
	int Time;
};

class CTrickDamageEffect_PROTO: public CTrickDamageEffect{
public:
	CTrickDamageEffect_PROTO(std::string _name, va_list args):CTrickDamageEffect(_name){
		Size1 = va_arg(args, int);
		Size2 = va_arg(args, int);
	}
	void DrawDamageEffect(CBattle* _battle, CBImgBank* _bimgbank, CRect _attackerR, CRect _targetR) const;	//����
private:
	int Size1;	//���������̑傫��
	int Size2;	//�ł�������ăq�b�g��������̑傫��
};




////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										////
////////////////////////////////////////////////