#include "../Define.h"
#include "BImgBank.h"


void CBImgBank::Init(){
	ImgBank.clear();
	for (int i=0; i<MAP_MAX; i++){
		BattleBackGround[i].clear();
		bbg_tag tmp;
		tmp.chipnum = -1; 
		tmp.gragh = 0; 
		BattleBackGround[i].push_back(tmp);
	}
}

void CBImgBank::LoadPic(const char *_path, const char _key[32], const char _kind[32]){
	Add(_key, LoadGraph(_path, true));
}

bool CBImgBank::Add(const char* _key, const int _img){
	
	if (mystrcmp(_key, "NULL")){
		ErrorDx("Error->BImgBank::LoadPic->You can't use \"NULL\" for PicKey(cancel):%s", __FILE__, __LINE__, _key);
		return false;
	}
	
	if (ImgBank[_key]!=0){
		ErrorDx("Error->BImgBank->AlreadyResiteredImageKey (cancel):%s", __FILE__, __LINE__, _key);
		return false;
	}

	ImgBank[_key] = _img;		//上記重複チェックif文で自動insertしてしまっているため、ここでinsertすると内部エラーが発生し何も起こらなくなってしまう
	return true;
}

int CBImgBank::GetImg(const char* _key){
	if (mystrcmp(_key, "NULL")) return 0;

	int tmp = ImgBank[_key];
	if (tmp != 0){
		return tmp;
	}else{
		ErrorDx("BImgBank->GetImg->Not Found:%s", _key);		
		return 0;
	}
}

int CBImgBank::GetBattleBackGround(int _mapnum, int _chipnum){
	MAP_MAX_CHECK(_mapnum, 0);
	if (BattleBackGround[_mapnum].size()==0){
		ErrorDx("InternalError->BattleBackGround[_mapnum].size()==0 _mapnum:%d", __FILE__, __LINE__, _mapnum);
		return 0;
	}else if (_chipnum<-1 || _chipnum>255){
		ErrorDx("Error->_chipnum should between(-1,255) (return 0):%d", __FILE__, __LINE__, _chipnum);
		return BattleBackGround[_mapnum][0].gragh;
	}

	for (unsigned int i=0; i<BattleBackGround[_mapnum].size(); i++){
		if (BattleBackGround[_mapnum][i].chipnum == _chipnum) {
			return BattleBackGround[_mapnum][i].gragh;
		}
	}
	return BattleBackGround[_mapnum][0].gragh;
}

void CBImgBank::SetBattleBackGround(const char* _key,int _mapnum, int _chipnum){
	MAP_MAX_CHECK(_mapnum, );
	if (_chipnum<-1 || _chipnum>255){
		ErrorDx("Error->_chipnum should between(-1,255) (return 0):%d", __FILE__, __LINE__, _chipnum);
		return;
	}

	int graph = GetImg(_key);
	for (unsigned int i=0; i<BattleBackGround[_mapnum].size(); i++){
		if (BattleBackGround[_mapnum][i].chipnum == _chipnum) {
			BattleBackGround[_mapnum][i].gragh = graph;
			return;
		}
	}

	bbg_tag tmp;
	tmp.chipnum = _chipnum; 
	tmp.gragh = graph; 
	BattleBackGround[_mapnum].push_back(tmp);
}