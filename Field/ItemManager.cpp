#include "ItemManager.h"


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

}


void CItemManager::Add(const char* _name, const char* _kind, int _salePrice, int _ownLimit, std::vector<std::string> _flags) {
	
	CItem* newItem = new CItem();




}
