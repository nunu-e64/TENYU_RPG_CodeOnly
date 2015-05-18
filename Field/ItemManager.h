////多重インクルード防止（インクルードガード）//	//pragmaonceと同じ意味だがこちらはコンパイラに依存しない
#ifndef ITEMMANAGER_H								////
#define ITEMMANAGER_H								////
////////////////////////////////////////////////

#include <vector>
#include <map>
#include <string>
#include "Item.h"


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

	void Add(const char* _name, const char* _kind, int _salePrice, int _ownLimit, std::vector<std::string> _flags);


private:
	std::map <std::string, CItem*> ItemBank;
	//bool ItemBankLock;	要素のポインタを取得することはないのでロックは不要

	enum item_tag {
		NORMAL,
		WEAPON,
		ACCECERLY
	};
	std::map < std::string, item_tag > item_tag_map;

	struct hoge_tag2 {
		enum type { AA, BB, CC, NUM };
		std::map < std::string, type > converter;
		hoge_tag2() {
		}
	}hoge_tag2;
	//std::map < std::string, hoge_tag2::type > hoge_tag2::converter;

	ENUM(hoge_tag, AAA, BBB, CCC, DDD, EEE);

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