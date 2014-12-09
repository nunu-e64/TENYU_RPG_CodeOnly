#include "../Define.h"
#include "BImgBank.h"


void CBImgBank::Init(){
	ImgBank.clear();
	for (int i=0; i<MAP_MAX; i++){
		BattleBackGround[i].clear();
		BattleBackGround[i].insert(std::pair<int,int>(-1,0));
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

void CBImgBank::SetBattleBackGround(const char* _key,int _mapnum, int _chipnum){
	MAP_MAX_CHECK(_mapnum, );
	if (_chipnum<-1 || _chipnum>255){
		ERRORDX("%s:_chipnum should between(-1,255) (return 0):%d", _key, _chipnum);
		return;
	}

	BattleBackGround[_mapnum][_chipnum] = GetImg(_key);
}

int CBImgBank::GetBattleBackGround(int _mapnum, int _chipnum){
	MAP_MAX_CHECK(_mapnum, 0);
	
	if (BattleBackGround[_mapnum].empty()){
		ErrorDx("InternalError->BattleBackGround[_mapnum].size()==0 _mapnum:%d", __FILE__, __LINE__, _mapnum);
		return 0;
	}else if (_chipnum<-1 || _chipnum>255){
		ERRORDX("%d:_chipnum'%d' should between(-1,255) (return 0)", _mapnum, _chipnum);
		return BattleBackGround[_mapnum][-1];
	}

	if (BattleBackGround[_mapnum].find(_chipnum)!=BattleBackGround[_mapnum].end()) {
		return BattleBackGround[_mapnum][_chipnum];
	} else {
		return BattleBackGround[_mapnum][-1];
	}
}