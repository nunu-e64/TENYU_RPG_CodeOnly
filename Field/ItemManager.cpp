#include "../Define.h"
#include "ItemManager.h"


void CItemManager::Init() {
	Clear();

	DEBUGDX("MACROstart");
	ENUM(hoge_tag, hoge_tag_map, AAA,BBB,CCC,DDD,EEE);
	DEBUGDX("MACROfin: %s: %d, %s:%d", STR(AAA), AAA, STR(hoge_tag::NUM), hoge_tag::NUM);

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

	std::string hoge;
	item_tag kind = item_tag_map[_kind];


	if (mystrcmp(_kind, STR(NORMAL))) {}
	item_tag_map[STR(WEAPON)] = WEAPON;
	
	/*

	やりたいことは、
	ENUM(tagname, hoge0, hoge1, hoge2, ....)
	enum item_tag {__VA_ARG__}
	#_VA_ARG__

	１．enumで管理、ifでchar*をチェックしてintに変換
	２．map<char* , int>とenumで管理。mapでintに変換
		・項目増やすのが手間？
			・enum増やす→map増やす　∴if文書き足すよりも楽、平文を一か所でしか使わない

	３．intではなくstringでkindを保持する
		・入力ミスの確認のため結局mapかifでチェックが必要になる

	∴２を採用→一般化してライブラリに足したい
	create(hoge)
	enum不要説？
	利用時はkind = HOGEHOGE then の形式なので　kind＝hoge(HOGEHOGE)
	どのみちHOGEHOGE：enumかconstcher*は必要になる
	ライブラリ関数内でconstの定義はできない
	enumは作ってもらわねばならない
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
