#include "../Define.h"
#include "ItemManager.h"


void CItemManager::Init() {
	Clear();

	//DEBUGDX("MACRO: %s: %d, %s:%d", STR(AAA), AAA, STR(hoge_tag::NUM), hoge_tag::NUM);
	
	hoge_tag::type i = hoge_tag::AAA;
	hoge_tag::type j = hoge_tag.converter["AA"];

	DEBUGDX("key:%s, value:%d", "AAA", hoge_tag.converter["AAA"]);
	DEBUGDX("key:%s, value:%d", "BBB", hoge_tag.converter["BBB"]);
	DEBUGDX("key:%s, value:%d", "NUM", hoge_tag::NUM);

	DEBUGDX("key:%s, value:%d", "AA",  hoge_tag2.converter["AA"]);
	DEBUGDX("key:%s, value:%d", "BB",  hoge_tag2.converter["BB"]);
	DEBUGDX("key:%s, value:%d", "NUM", hoge_tag2::NUM);

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
