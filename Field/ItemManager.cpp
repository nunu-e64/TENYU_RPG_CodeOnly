#include "ItemManager.h"
#include "Item.h"


void CItemManager::Init() {
	Clear();
}

void CItemManager::Clear(){

	//イテレータ使ってすべてdeleteしなくちゃいけない 
	std::map <std::string, CItem*>::iterator it = ItemBank.begin();
	while (it != ItemBank.end()) {
		delete (*it).second;
		++it;	//イテレータの指す先を無効化しているがeraseしたわけではないのでイテレータは変動していない＠。ただし保障されていないため危険ではある。
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

