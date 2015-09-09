////多重インクルード防止（インクルードガード）//	//pragmaonceと同じ意味だがこちらはコンパイラに依存しない
#ifndef ALCHEMISTMANAGER_H						////
#define ALCHEMISTMANAGER_H						////
////////////////////////////////////////////////

#include "ShopManager.h"

class CAlchemistManager : public CShopManager {
public:
	static CAlchemistManager* GetInstance() {
		static CAlchemistManager instance;
		return &instance;
	}
	~CAlchemistManager() {
		DESTRUCTED;
	}

	bool OpenShop(int _index);

	bool Main();

private:

/*	struct ShopMenu {
		std::vector	<CItem*> ItemList;	//開店時に作る商品棚
		std::vector<int> Basket;	//数量を記録する買い物かご
		int SumPrice;

		int Cursor;
		bool IsConfirm;
		CItemManager* ItemManager;

		void Move(int _dir);
		bool Buy();
	}ShopMenu;
*/
	//単一性を保証(Singleton)////////////////////////////////
	CAlchemistManager() {
		CONSTRUCTED;
	}
	CAlchemistManager(const CAlchemistManager& hoge);
	CAlchemistManager& operator=(const CAlchemistManager& hoge);
	/////////////////////////////////////////////////////////

};

////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////