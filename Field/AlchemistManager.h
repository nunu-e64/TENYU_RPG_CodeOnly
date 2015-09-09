////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//	//pragmaonce�Ɠ����Ӗ�����������̓R���p�C���Ɉˑ����Ȃ�
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
		std::vector	<CItem*> ItemList;	//�J�X���ɍ�鏤�i�I
		std::vector<int> Basket;	//���ʂ��L�^���锃��������
		int SumPrice;

		int Cursor;
		bool IsConfirm;
		CItemManager* ItemManager;

		void Move(int _dir);
		bool Buy();
	}ShopMenu;
*/
	//�P�ꐫ��ۏ�(Singleton)////////////////////////////////
	CAlchemistManager() {
		CONSTRUCTED;
	}
	CAlchemistManager(const CAlchemistManager& hoge);
	CAlchemistManager& operator=(const CAlchemistManager& hoge);
	/////////////////////////////////////////////////////////

};

////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										////
////////////////////////////////////////////////