////多重インクルード防止（インクルードガード）//
#ifndef TRICKDAMAGEEFFECT_H					////
#define TRICKDAMAGEEFFECT_H					////
////////////////////////////////////////////////

#include <map>
#include <string>
//#include "Battle.h"

class CBattle;
class CBImgBank;
class CTrickDamageEffect{ //戦闘ダメージエフェクト用のインターフェース
public: 
	CTrickDamageEffect(std::string _name){
		Name = _name;
	}
	virtual void DrawDamageEffect(CBattle* _battle, CBImgBank* _bimgbank, CRect _attackerR, CRect _targetR) const =0;
	std::string GetName() const {return Name;}
protected:
	std::string Name;	//エフェクトの名前　例：RED_BOMB, BLUE_BOMB, BIG_SPREAD,　SMALL_SPREAD
	//std::string TypeName;	//Typeの名前　例：BOMB, SPREAD
};


class CTrickDamageEffect_BOMB: public CTrickDamageEffect{
public:
	CTrickDamageEffect_BOMB(std::string _name, va_list args):CTrickDamageEffect(_name){
		Color = va_arg(args, int);
		Size = va_arg(args, int);
		Time = va_arg(args, int);
	}
	void DrawDamageEffect(CBattle* _battle, CBImgBank* _bimgbank, CRect _attackerR, CRect _targetR) const;	//共通
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
	void DrawDamageEffect(CBattle* _battle, CBImgBank* _bimgbank, CRect _attackerR, CRect _targetR) const;	//共通
private:
	int Size1;	//収束光球の大きさ
	int Size2;	//打ちだされてヒットする光球の大きさ
};




////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////