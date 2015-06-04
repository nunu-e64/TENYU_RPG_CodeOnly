////多重インクルード防止（インクルードガード）//	//pragmaonceと同じ意味だがこちらはコンパイラに依存しない
#ifndef ITEMMANAGER_H								////
#define ITEMMANAGER_H								////
////////////////////////////////////////////////

#include <vector>
#include <map>
#include <string>
//#include "Item.h"

class CItem;
class CConsumptionItem;
class CAccessoryItem;
class CKeyItem;
class CMaterialItem;

class CItemManager {
public:
	static CItemManager* GetInstance() {
		static CItemManager instance;
		return &instance;
	}

	~CItemManager() {
		DESTRUCTED;
	}

	void Init();
	void Clear();

	void AddItem			(const char* _name, const char* _kind, int _ownLimit, int _price, bool _sellable);
	void AddConsumptionItem (const char* _name, const char* _kind, int _ownLimit, int _price, bool _sellable, bool _battleUsable, int _waitTime, const char* _target, std::vector<std::pair<std::string, int> > _effectSet);
	void AddAccessoryItem	(const char* _name, const char* _kind, int _ownLimit, int _price, bool _sellable, std::vector < std::pair<std::string, int> > _material);
	void AddKeyItem			(const char* _name, const char* _kind, int _ownLimit, int _price, bool _sellable);
	void AddMaterialItem	(const char* _name, const char* _kind, int _ownLimit, int _price, bool _sellable);

	void SetAccessoryEffect(const char* _name, std::vector<sideEffect_tag> _effectSet);

	ENUM(item_tag, NOKIND, CONSUMPTION, ACCESSORY, KEY, MATERIAL);

private:

	std::map <std::string, CItem*> ItemBank;

	std::map <std::string, CConsumptionItem*> ConsumptionItemBank;
	std::map <std::string, CAccessoryItem*> AccessoryItemBank;
	std::map <std::string, CKeyItem*> KeyItemBank;
	std::map <std::string, CMaterialItem*> MaterialItemBank;

	//bool ItemBankLock;	要素のポインタを取得することはない(要素自体がポインタの時は問題ない)のでロックは不要

	
	//単一性を保証(Singleton)////////////////////////////////
	CItemManager() {
		CONSTRUCTED;
	}
	CItemManager(const CItemManager& hoge);
	CItemManager& operator=(const CItemManager& hoge);
	/////////////////////////////////////////////////////////


};

////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////