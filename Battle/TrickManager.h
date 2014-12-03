////多重インクルード防止（インクルードガード）//
#ifndef TRICKMANAGER_H						////
#define TRICKMANAGER_H						////
////////////////////////////////////////////////

#include <vector>
#include <map>
#include <string>
#include "TrickDamageEffect.h"

class CTrickManager{
public:

	static CTrickManager* GetInstance(){
		static CTrickManager TrickManager;
		return &TrickManager;
	}

	~CTrickManager(){
		
	}

	void Clear();
	void Add(trick_tag _trick);
	void Add(char _name[32], int _power, int _cost, trick_tag::targetType_tag _targetType, int _sideeffectnum, ...);

	trick_tag const* GetTrick(const char _name[32]);


	//ダメージエフェクト関係
		void CreateDamageEffect(std::string _typeName);
		int GetTrickDamageEffectIndex(std::string _name);
		void DrawEffect(int _effectIndex, CBattle* _battle, CBImgBank* _bimgbank, CRect _attackerR, CRect _targetR) {
			//TrickDamageEffectBank[_effectIndex]->DrawDamageEffect(_battle, );
		}


private:
	std::map <char256, trick_tag> TrickBank;
	trick_tag Trick_dammy;

	std::vector <const CTrickDamageEffect*> TrickDamageEffectBank;
	//ポインタにしないと多態性が発動しないよな？

		

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