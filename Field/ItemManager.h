////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//	//pragmaonce�Ɠ����Ӗ�����������̓R���p�C���Ɉˑ����Ȃ�
#ifndef ITEMMANAGER_H								////
#define ITEMMANAGER_H								////
////////////////////////////////////////////////

#include <vector>
#include <map>
#include <string>
#include "Item.h"


#define STR(_var) #_var
#define ENUM(_name, _listname, ...) \
	DEBUGDX(#__VA_ARGS__);	\
	enum _name {__VA_ARGS__, NUM}; \
	std::map <std::string, _name> _listname;	\
	createEnumMap(_listname, #__VA_ARGS__, _name::NUM, __VA_ARGS__);

template <class T>inline void createEnumMap(std::map<std::string, T> _map, char* _list, int _num, ...) {
	char* cntx;		//strtok_s�p�̎G�p	
	char *delim = ", ";//�f���~�^�i�����n����j�����ł̓J���}�Ƌ�
	char* tmpKey = new char[255];  
	char* copy = new char[255];

	mystrcpy(copy, _list);

	va_list args;
	va_start(args, _num);

	DEBUGDX("YET:%s", copy);
	tmpKey = strtok_s(copy, delim, &cntx);
	DEBUGDX("OK:%s", copy);
	DEBUGDX("%s", tmpKey);

	if ((tmpKey = strtok_s(_list, ", ", &cntx)) != NULL) {
		DEBUGDX("%s", tmpKey);
		_map[std::string(tmpKey)] = va_arg(args, T);
		DEBUGDX("%d", (int)_map[tmpKey]);
	}
	for (int i = 1; i < _num; i++) {
		if ((tmpKey = strtok_s(NULL, ", ", &cntx)) != NULL) {
			_map[tmpKey] = va_arg(args, T);
			DEBUGDX("%d", (int)_map[tmpKey]);
		}
	}
	va_end(args);

	delete[] tmpKey;
}

class CItemManager{
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


private :
	std::map <std::string, CItem*> ItemBank;
	//bool ItemBankLock;	�v�f�̃|�C���^���擾���邱�Ƃ͂Ȃ��̂Ń��b�N�͕s�v

	enum item_tag {
		NORMAL, 
		WEAPON,
		ACCECERLY
	};
	std::map < std::string, item_tag > item_tag_map;


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