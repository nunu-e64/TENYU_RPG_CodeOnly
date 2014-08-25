////多重インクルード防止（インクルードガード）//
#ifndef TRICKMANAGER_H						////
#define TRICKMANAGER_H						////
////////////////////////////////////////////////

#include <map>

class CTrickManager{
public:
	
	~CTrickManager(){
		
	}
	void Add(trick_tag _trick);
	void Add(char _name[32], int _power, int _cost, int _sideeffectnum, ...);
	void Clear(){TrickBank.clear();}

	trick_tag const* GetTrick(const char _name[32]);

private:
	std::map <char256, trick_tag> TrickBank;
	trick_tag Trick_dammy;
};


////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////