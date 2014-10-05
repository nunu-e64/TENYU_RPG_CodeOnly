#include "../Define.h"
#include "BImgBank.h"


void CBImgBank::LoadPic(const char *_path, const char _key[32], const char _kind[32]){
	Add(_key, LoadGraph(_path, true));
}

bool CBImgBank::Add(const char* _key, const int _img){
	
	if (mystrcmp(_key, "NULL")){
		ErrorDx("Error->CBImgManager::LoadPic->You can't use \"NULL\" for PicKey(cancel):%s", __FILE__, __LINE__, _key);
		return false;
	}
	
	if (ImgBank[_key]!=0){
		ErrorDx("Error->CBImgManager->AlreadyResiteredImageKey (cancel):%s", __FILE__, __LINE__, _key);
		return false;
	}

	ImgBank.insert( std::map<std::string, int>::value_type( _key, _img) );
}

int CBImgBank::GetImg(const char* _key){
	int tmp = ImgBank[_key];
	if (tmp != 0){
		return tmp;
	}else{
		ErrorDx("BImgManager->GetImg->Not Found:%s", _key);
		return NULL;
	}
}
