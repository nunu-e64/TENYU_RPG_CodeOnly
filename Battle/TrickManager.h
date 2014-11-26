////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#ifndef TRICKMANAGER_H						////
#define TRICKMANAGER_H						////
////////////////////////////////////////////////

#include <map>

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
	void Clear(){TrickBank.clear();}

	trick_tag const* GetTrick(const char _name[32]);

private:
	std::map <char256, trick_tag> TrickBank;
	trick_tag Trick_dammy;

	//�P�ꐫ��ۏ�(Singleton)
		CTrickManager(){}
		CTrickManager(const CTrickManager& hoge);
		CTrickManager& operator=(const CTrickManager& hoge);
};


////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										////
////////////////////////////////////////////////