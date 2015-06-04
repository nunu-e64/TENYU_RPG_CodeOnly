////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//	//pragmaonce�Ɠ����Ӗ�����������̓R���p�C���Ɉˑ����Ȃ�
#ifndef ITEM_H								////
#define ITEM_H								////
////////////////////////////////////////////////

#include "ItemManager.h"

class CItem{	//�C���^�[�t�F�[�X�N���X
public:
	CItem() {
		CONSTRUCTED;
		Name = "";
		Kind = CItemManager::item_tag::NOKIND;
		Price = 0;
		OwnLimit = -1;
	}

	std::string Name;
	CItemManager::item_tag::type Kind;
	int OwnLimit;		//���������@0�ŏ����s�@-1�Ŗ�����
	int Price;			//���p���i
	bool Sellable;

};



class CConsumptionItem: public CItem {
public:
	bool BattleUsable;	//�퓬�����p��
	int WaitTime;		//�퓬���g�p��̑ҋ@����

	std::vector<std::pair<sideEffect_tag::type_tag::type, int> > effectSet;	//�X�e�[�^�X���ƌ��ʒl(%)
	sideEffect_tag::target_tag::type Target;
};

class CAccessoryItem: public CItem {
public:
	std::vector < std::pair<std::string, int> > MaterialSet;	//�����ɕK�v�ȑf�ރA�C�e�����ƌ�

};

class CKeyItem: public CItem {

};

class CMaterialItem: public CItem {

};

////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										////
////////////////////////////////////////////////