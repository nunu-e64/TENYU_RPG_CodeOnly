#include "ItemManager.h"


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

}


void CItemManager::Add(const char* _name, const char* _kind, int _salePrice, int _ownLimit, std::vector<std::string> _flags) {
	
	CItem* newItem = new CItem();




}
