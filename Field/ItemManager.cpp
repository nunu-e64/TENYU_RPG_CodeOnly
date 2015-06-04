#include "ItemManager.h"
#include "Item.h"


void CItemManager::Init() {
	Clear();
}

void CItemManager::Clear(){

	//�C�e���[�^�g���Ă��ׂ�delete���Ȃ����Ⴂ���Ȃ� 
	std::map <std::string, CItem*>::iterator it = ItemBank.begin();
	while (it != ItemBank.end()) {
		delete (*it).second;
		++it;	//�C�e���[�^�̎w����𖳌������Ă��邪erase�����킯�ł͂Ȃ��̂ŃC�e���[�^�͕ϓ����Ă��Ȃ����B�������ۏႳ��Ă��Ȃ����ߊ댯�ł͂���B
	}
	ItemBank.clear();

	ConsumptionItemBank.clear();
	AccessoryItemBank.clear();
	KeyItemBank.clear();
	MaterialItemBank.clear();

}


void CItemManager::AddItem(const char* _name, const char* _kind, int _ownLimit, int _price, bool _sellable) {
	

	if (item_tag.converter.find(_kind) == item_tag.converter.end()) {
		WARNINGDX("Check Kind Name[%s].(not add this item) :%s", _kind, _name);
		return;
	}

	if (ItemBank.find(_name) != ItemBank.end()) {

		CItem* newItem = new CItem();

		newItem->Kind = item_tag.converter[_kind];
		newItem->Name = _name;
		newItem->OwnLimit = _ownLimit;
		newItem->Price = _price;
		
		newItem->Sellable = _sellable;

		ItemBank[_name] = newItem;

	} else {
		WARNINGDX("Already Exist Item Name :%s", _name);
	}
}

