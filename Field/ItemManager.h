////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//	//pragmaonce�Ɠ����Ӗ�����������̓R���p�C���Ɉˑ����Ȃ�
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

	//bool ItemBankLock;	�v�f�̃|�C���^���擾���邱�Ƃ͂Ȃ�(�v�f���̂��|�C���^�̎��͖��Ȃ�)�̂Ń��b�N�͕s�v

	
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