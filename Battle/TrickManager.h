////多重インクルード防止（インクルードガード）//
#ifndef TRICKMANAGER_H						////
#define TRICKMANAGER_H						////
////////////////////////////////////////////////

#include <map>
#include <string>

class CTrickDamageEffect{
public: 
	virtual const void DrawDamageEffect()=0;
	const std::string GetName(){return Name;}
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

	const void DrawDamageEffect();	//Colorとか自由に使用
private:
	int Color;
	int Size;
	int Time;
};

class CTrickManager{
public:

	static CTrickManager* GetInstance(){
		static CTrickManager TrickManager;
		return &TrickManager;
	}

	~CTrickManager(){
		
	}
	void Add(trick_tag _trick);
	void Add(char _name[32], int _power, int _cost, trick_tag::targetType_tag _targetType, int _sideeffectnum, ...);
	void Clear(){TrickBank.clear();	
		
		//イテレータ使ってすべてdeleteしなくちゃいけない　
		TrickDamageEffectBank.clear();
	}
	trick_tag const* GetTrick(const char _name[32]);


	void CreateDamageEffect(std::string _typeName);
	int GetTrickDamageEffectIndex(std::string _name){
		for (unsigned int i=0; i<TrickDamageEffectBank.size(); i++){
			if (TrickDamageEffectBank[i]->GetName()==_name){
				return i;
			}
		}
		return -1;
	}
	void DrawEffect(int _effectIndex){
		TrickDamageEffectBank[_effectIndex]->DrawDamageEffect();
	}


private:
	std::map <char256, trick_tag> TrickBank;
	trick_tag Trick_dammy;

	std::vector <const CTrickDamageEffect* const> TrickDamageEffectBank;
	//ポインタにしないと多態性が発動しないかな？

		

	//単一性を保証(Singleton)////////////////////////////////
		CTrickManager(){}
		CTrickManager(const CTrickManager& hoge);
		CTrickManager& operator=(const CTrickManager& hoge);
	/////////////////////////////////////////////////////////

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