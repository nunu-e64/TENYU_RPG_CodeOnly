#include "ItemManager.h"
#include "Item.h"


void CItemManager::Init() {
	Clear();
}

void CItemManager::Clear(){
	DEBUGDX("CLEAR");

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

	PlayerItemBag.clear();

}


bool CItemManager::AddItem(CItem* _newItem, const char* _name, item_tag::type _kind, int _ownLimit, int _price, bool _sellable) {

	if (ItemBank.find(_name) == ItemBank.end()) {

		_newItem->Kind = _kind;
		_newItem->Name = _name;
		_newItem->OwnLimit = _ownLimit;
		_newItem->Price = _price;
		
		_newItem->Sellable = _sellable;

		ItemBank[_name] = _newItem;
		DEBUGDX("newItem:%s:%d", _name, (int)ItemBank[_name]);
		return true;

	} else {
		WARNINGDX("Already Exist Item Name :%s", _name);
		return false;
	}
}

void CItemManager::AddConsumptionItem(const char* _name, int _ownLimit, int _price, bool _sellable, bool _battleUsable, int _waitTime, const char* _target, std::vector<std::pair<std::string, int> > _effectSet) {

	if (ConsumptionItemBank.find(_name) == ConsumptionItemBank.end()) {

		CConsumptionItem* newItem = new CConsumptionItem();

		sideEffect_tag tmpEffect;
		newItem->BattleUsable = _battleUsable;
		newItem->WaitTime = _waitTime;

		if (tmpEffect.target_tag.exist(_target)) {
			newItem->Target = tmpEffect.target_tag.converter[_target];
		} else {
			WARNINGDX("Don't match any key[sideEffect_tag::target_tag] name:%s, key:%s", _name, _target);
			delete newItem;
			return;
		}

		for (unsigned int i = 0; i < _effectSet.size(); i++) {
			if (tmpEffect.type_tag.exist(_effectSet[i].first)) {
				newItem->effectSet.push_back(std::pair<sideEffect_tag::type_tag::type, int>(tmpEffect.type_tag.converter[_effectSet[i].first], _effectSet[i].second));
			} else{
				WARNINGDX("Don't match any key[sideEffect_tag::type_tag] name:%s, key:%s", _name, _effectSet[i].first.c_str());
			}
		}

		if (AddItem(newItem, _name, item_tag::CONSUMPTION, _ownLimit, _price, _sellable)) {
			ConsumptionItemBank[_name] = newItem;
			return;
		} else {
			delete newItem;
		}

	} else {
		WARNINGDX("Already Exist Item Name :%s", _name);
		return;
	}

}

void CItemManager::AddAccessoryItem(const char* _name, int _ownLimit, int _price, bool _sellable, std::vector < std::pair<std::string, int> > _materialSet) {

	if (AccessoryItemBank.find(_name) == AccessoryItemBank.end()) {

		CAccessoryItem* newItem = new CAccessoryItem();

		for (unsigned int i = 0; i < _materialSet.size(); i++) {
			if (!(MaterialItemBank.find(_materialSet[i].first) != MaterialItemBank.end())) {
				WARNINGDX("Not Found Material:%s, ItemName:%s", _materialSet[i].first.c_str(), _name);
				continue;
			}
			if (_materialSet[i].second <= 0) {
				WARNINGDX("Material Num > 0 :%d, ItemName:%s", _materialSet[i].second, _name);
				continue;
			}
			newItem->MaterialSet.push_back(_materialSet[i]);
		}

		if (AddItem(newItem, _name, item_tag::ACCESSORY, _ownLimit, _price, _sellable)) {
			AccessoryItemBank[_name] = newItem;
			return;
		} else {
			delete newItem;
		}

	} else {
		WARNINGDX("Already Exist Item Name :%s", _name);
		return;
	}

}

void CItemManager::AddKeyItem(const char* _name, int _ownLimit, int _price, bool _sellable) {

	if (KeyItemBank.find(_name) == KeyItemBank.end()) {

		CKeyItem* newItem = new CKeyItem();
		if (AddItem(newItem, _name, item_tag::KEY, _ownLimit, _price, _sellable)) {
			KeyItemBank[_name] = newItem;
			return;
		} else {
			delete newItem;
		}

	} else {
		WARNINGDX("Already Exist Item Name :%s", _name);
		return;
	}


}
void CItemManager::AddMaterialItem(const char* _name, int _ownLimit, int _price, bool _sellable) {

	if (MaterialItemBank.find(_name) == MaterialItemBank.end()) {

		CMaterialItem* newItem = new CMaterialItem();
		if (AddItem(newItem, _name, item_tag::MATERIAL, _ownLimit, _price, _sellable)) {
			MaterialItemBank[_name] = newItem;
			return;
		} else {
			delete newItem;
		}

	} else {
		WARNINGDX("Already Exist Item Name :%s", _name);
		return;
	}

}

void CItemManager::SetAccessoryEffect(const char* _name, std::vector<sideEffect_tag> _effectSet) {


}
bool CItemManager::IncPlayerItem(std::string _name, int _num) {
	bool result = true;
	
	for (int i = 0; i < _num; i++) {
		result = IncPlayerItem(_name) && result;
	}
	return result;

}
bool CItemManager::IncPlayerItem(std::string _name) {

	if (ItemBank.find(_name) == ItemBank.end()) {
		ERRORDX("Not Found Item. :%s", _name.c_str());
		return false;

	} else if (PlayerItemBag.find(_name) == PlayerItemBag.end()) {
		if (ItemBank[_name]->OwnLimit != 0) {
			PlayerItemBag[_name] = 1;
			DEBUGDX("%s:%d", _name.c_str(), PlayerItemBag[_name]);
			return true;
		} else {
			return false;
		}

	} else {
		if (PlayerItemBag[_name] + 1 <= ItemBank[_name]->OwnLimit || ItemBank[_name]->OwnLimit == -1) {
			++PlayerItemBag[_name];
			DEBUGDX("%s:%d", _name.c_str(), PlayerItemBag[_name]);
			return true;
		} else {
			return false;
		}
	}

}

bool CItemManager::DecPlayerItem(std::string _name, int _num) {
	bool result = true;

	for (int i = 0; i < _num; i++) {
		result = DecPlayerItem(_name) && result;
	}
	return result;
}
bool CItemManager::DecPlayerItem(std::string _name) {

	if (ItemBank.find(_name) == ItemBank.end()) {
		ERRORDX("Not Found Item. :%s", _name.c_str());
		return false;

	} else if (PlayerItemBag.find(_name) == PlayerItemBag.end()) {
		return false;

	} else {
		if (PlayerItemBag[_name] > 0) {
			--PlayerItemBag[_name];
			DEBUGDX("%s:%d", _name.c_str(), PlayerItemBag[_name]);
			return true;
		} else {
			return false;
		}
	}
}

CItem* CItemManager::GetItem(std::string _name) {

	if (ItemBank.find(_name) == ItemBank.end()) {
		ERRORDX("Not Found Item. :%s", _name.c_str());
		return NULL;
	} else {
		return ItemBank[_name];
	}
}

int CItemManager::GetPlayerItemNum(std::string _name) {

	if (ItemBank.find(_name) == ItemBank.end()) {
		WARNINGDX("Not Found Item. :%s", _name.c_str());
		return 0;
	} else if (PlayerItemBag.find(_name) == PlayerItemBag.end()) {
		return 0;
	} else {
		return PlayerItemBag[_name];
	}
}
