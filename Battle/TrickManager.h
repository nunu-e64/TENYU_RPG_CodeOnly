////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
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


	//�_���[�W�G�t�F�N�g�֌W
		void CreateDamageEffect(std::string _typeName);
		int GetTrickDamageEffectIndex(std::string _name);
		void DrawEffect(int _effectIndex, CBattle* _battle, CBImgBank* _bimgbank, CRect _attackerR, CRect _targetR) {
			//TrickDamageEffectBank[_effectIndex]->DrawDamageEffect(_battle, );
		}


private:
	std::map <char256, trick_tag> TrickBank;
	trick_tag Trick_dammy;

	std::vector <const CTrickDamageEffect*> TrickDamageEffectBank;
	//�|�C���^�ɂ��Ȃ��Ƒ��Ԑ����������Ȃ���ȁH

		

	//�P�ꐫ��ۏ�(Singleton)////////////////////////////////
		CTrickManager(){}
		CTrickManager(const CTrickManager& hoge);
		CTrickManager& operator=(const CTrickManager& hoge);
	/////////////////////////////////////////////////////////

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