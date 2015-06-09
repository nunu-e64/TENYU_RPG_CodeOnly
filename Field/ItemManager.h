////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//	//pragmaonce�Ɠ����Ӗ�����������̓R���p�C���Ɉˑ����Ȃ�
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

	int GetPlayerItemNum(std::string _name);			//�������Ă���A�C�e�������擾

	std::vector <std::string> GetBattleItemNameList();	//�������Ă��闘�p�\�ȏ���A�C�e���̃��X�g���擾


	void DebugShowAllItem();	//DEBUG: 
	void DebugShowAllPlayerItem();	//DEBUG: 

private:

	bool IncPlayerItem(std::string _name);	//true...�������� false...��������or�G���[
	bool DecPlayerItem(std::string _name);	//true...�������� false...����0or�G���[


	std::map <std::string, CItem*> ItemBank;
	std::map <std::string, CConsumptionItem*> ConsumptionItemBank;
	std::map <std::string, CAccessoryItem*> AccessoryItemBank;
	std::map <std::string, CKeyItem*> KeyItemBank;
	std::map <std::string, CMaterialItem*> MaterialItemBank;

	std::map <std::string, int> PlayerItemBag;	//�����A�C�e���̌�


	//�P�ꐫ��ۏ�(Singleton)////////////////////////////////
	CItemManager() {
		CONSTRUCTED;
	}
	CItemManager(const CItemManager& hoge);
	CItemManager& operator=(const CItemManager& hoge);
	/////////////////////////////////////////////////////////


};

////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										////
////////////////////////////////////////////////