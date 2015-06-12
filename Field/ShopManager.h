////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//	//pragmaonce�Ɠ����Ӗ�����������̓R���p�C���Ɉˑ����Ȃ�
#ifndef SHOPMANAGER_H						////
#define SHOPMANAGER_H						////
////////////////////////////////////////////////

class CItem;
class CItemManager;

class CShopManager {
public:
	static CShopManager* GetInstance() {
		static CShopManager instance;
		return &instance;
	}
	~CShopManager() {
		DESTRUCTED;
	}

	void Init();
	bool AddShop(int _index, std::vector<std::string> _itemList);
	std::vector <std::string> GetShop(int _index);

	bool OpenShop(int _index);
	bool IsOpen();

	bool Main();
	void Draw();

private:

	std::map<int, std::vector <std::string> > ShopBank;

	int currentOpenShopIndex;

	struct ShopMenu {
		std::vector	<CItem*> ItemList;	//�J�X���ɍ�鏤�i�I
		std::vector<int> Basket;	//���ʂ��L�^���锃��������
		int SumPrice;

		int Cursor;
		bool IsConfirm;
		CItemManager* ItemManager;

		void Move(int _dir);
		void Draw();
		bool Buy();
	}ShopMenu;


	//�P�ꐫ��ۏ�(Singleton)////////////////////////////////
	CShopManager() {
		CONSTRUCTED;
	}
	CShopManager(const CShopManager& hoge);
	CShopManager& operator=(const CShopManager& hoge);
	/////////////////////////////////////////////////////////

};

////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										////
////////////////////////////////////////////////