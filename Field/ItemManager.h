////多重インクルード防止（インクルードガード）//	//pragmaonceと同じ意味だがこちらはコンパイラに依存しない
#ifndef ITEMMANAGER_H								////
#define ITEMMANAGER_H								////
////////////////////////////////////////////////


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

	ENUM(item_tag, NOKIND, CONSUMPTION, ACCESSORY, KEY, MATERIAL);

	void Init();
	void Clear();

	bool AddItem(CItem* _newItem, const char* _name, item_tag::type _kind, int _ownLimit, int _price, bool _sellable);
	void AddConsumptionItem (const char* _name, int _ownLimit, int _price, bool _sellable, bool _battleUsable, int _waitTime, const char* _target, std::vector<std::pair<std::string, int> > _effectSet);
	void AddAccessoryItem	(const char* _name, int _ownLimit, int _price, bool _sellable, std::vector < std::pair<std::string, int> > _materialSet);
	void AddKeyItem			(const char* _name, int _ownLimit, int _price, bool _sellable);
	void AddMaterialItem	(const char* _name, int _ownLimit, int _price, bool _sellable);
	void SetAccessoryEffect (const char* _name, std::vector<sideEffect_tag> _effectSet);

	bool IncPlayerItem(std::string _name, int _num);	
	bool DecPlayerItem(std::string _name, int _num);

	CItem* GetItem(std::string _name);
	CConsumptionItem* GetConsumptionItem(std::string _name);

	int GetPlayerItemNum(std::string _name);			//所持しているアイテム数を取得

	std::vector <std::string> GetBattleItemNameList();	//所持している利用可能な消費アイテムのリストを取得


	void DebugShowAllItem();	//DEBUG: 
	void DebugShowAllPlayerItem();	//DEBUG: 

private:

	bool IncPlayerItem(std::string _name);	//true...増加成功 false...所持制限orエラー
	bool DecPlayerItem(std::string _name);	//true...減少成功 false...既に0orエラー


	std::map <std::string, CItem*> ItemBank;
	std::map <std::string, CConsumptionItem*> ConsumptionItemBank;
	std::map <std::string, CAccessoryItem*> AccessoryItemBank;
	std::map <std::string, CKeyItem*> KeyItemBank;
	std::map <std::string, CMaterialItem*> MaterialItemBank;

	std::map <std::string, int> PlayerItemBag;	//所持アイテムの個数


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