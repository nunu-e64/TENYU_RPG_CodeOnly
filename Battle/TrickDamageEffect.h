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
	virtual void DrawDamageEffect(CBattle* _battle, CBImgBank* _bimgbank, CRect _attackerR, CRect _targetR) const =0;
	std::string GetName() const {return Name;}
protected:
	std::string Name;	//エフェクトの名前　例：RED_BOMB, BLUE_BOMB, BIG_SPREAD,　SMALL_SPREAD
	//std::string TypeName;	//Typeの名前　例：BOMB, SPREAD
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

	void DrawDamageEffect(CBattle* _battle, CBImgBank* _bimgbank, CRect _attackerR, CRect _targetR) const;	//Colorとか自由に使用
private:
	int Color;
	int Size;
	int Time;
};





//class CTDE_Spread:public CTrickDamageEffect{
//public:
//	void DrawDamageEffect();	//Colorとか自由に使用
//private:
//	std::string TypeName;
//	int Color;
//	int Size;
//	int Time;
//};
//



////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////