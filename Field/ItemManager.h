////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//	//pragmaonce�Ɠ����Ӗ�����������̓R���p�C���Ɉˑ����Ȃ�
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
	//bool ItemBankLock;	�v�f�̃|�C���^���擾���邱�Ƃ͂Ȃ��̂Ń��b�N�͕s�v

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