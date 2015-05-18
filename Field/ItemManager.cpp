#include "../Define.h"
#include "ItemManager.h"


void CItemManager::Init() {
	Clear();

	DEBUGDX("MACROstart");
	ENUM(hoge_tag, hoge_tag_map, AAA,BBB,CCC,DDD,EEE);
	DEBUGDX("MACROfin: %s: %d, %s:%d", STR(AAA), AAA, STR(hoge_tag::NUM), hoge_tag::NUM);

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

	std::string hoge;
	item_tag kind = item_tag_map[_kind];


	if (mystrcmp(_kind, STR(NORMAL))) {}
	item_tag_map[STR(WEAPON)] = WEAPON;
	
	/*

	��肽�����Ƃ́A
	ENUM(tagname, hoge0, hoge1, hoge2, ....)
	enum item_tag {__VA_ARG__}
	#_VA_ARG__

	�P�Denum�ŊǗ��Aif��char*���`�F�b�N����int�ɕϊ�
	�Q�Dmap<char* , int>��enum�ŊǗ��Bmap��int�ɕϊ�
		�E���ڑ��₷�̂���ԁH
			�Eenum���₷��map���₷�@��if���������������y�A�������ꂩ���ł����g��Ȃ�

	�R�Dint�ł͂Ȃ�string��kind��ێ�����
		�E���̓~�X�̊m�F�̂��ߌ���map��if�Ń`�F�b�N���K�v�ɂȂ�

	���Q���̗p����ʉ����ă��C�u�����ɑ�������
	create(hoge)
	enum�s�v���H
	���p����kind = HOGEHOGE then �̌`���Ȃ̂Ł@kind��hoge(HOGEHOGE)
	�ǂ݂̂�HOGEHOGE�Fenum��constcher*�͕K�v�ɂȂ�
	���C�u�����֐�����const�̒�`�͂ł��Ȃ�
	enum�͍���Ă����˂΂Ȃ�Ȃ�
	*/
	/*
	newItem->Name = _name;
	newItem->Kind = 
	newItem->
	newItem->
	newItem->
	newItem->
	*/
}
