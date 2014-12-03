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
	virtual void DrawDamageEffect(CBattle* _battle, CBImgBank* _bimgbank, CRect _attackerR, CRect _targetR) const =0;
	std::string GetName() const {return Name;}
protected:
	std::string Name;	//�G�t�F�N�g�̖��O�@��FRED_BOMB, BLUE_BOMB, BIG_SPREAD,�@SMALL_SPREAD
	//std::string TypeName;	//Type�̖��O�@��FBOMB, SPREAD
};


class CTrickDamageEffect_BOMB: public CTrickDamageEffect{
public:
	CTrickDamageEffect_BOMB(){
		Color = RED;
		Size = 10;
		Time = 20;
	}
	
	CTrickDamageEffect_BOMB(std::string _name, int _color=RED, int _size=10, int _time=20){
		Name = _name;
		Color = _color;
		Size = _size;
		Time = _time;
	}

	void DrawDamageEffect(CBattle* _battle, CBImgBank* _bimgbank, CRect _attackerR, CRect _targetR) const;	//Color�Ƃ����R�Ɏg�p
private:
	int Color;
	int Size;
	int Time;
};





//class CTDE_Spread:public CTrickDamageEffect{
//public:
//	void DrawDamageEffect();	//Color�Ƃ����R�Ɏg�p
//private:
//	std::string TypeName;
//	int Color;
//	int Size;
//	int Time;
//};
//



////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										////
////////////////////////////////////////////////