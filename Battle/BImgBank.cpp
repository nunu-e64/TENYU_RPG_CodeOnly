#include "../Define.h"
#include "BImgBank.h"


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

	ImgBank[_key] = _img;		//��L�d���`�F�b�Nif���Ŏ���insert���Ă��܂��Ă��邽�߁A������insert����Ɠ����G���[�������������N����Ȃ��Ȃ��Ă��܂�
	return true;
}

int CBImgBank::GetImg(const char* _key){
	int tmp = ImgBank[_key];
	if (tmp != 0){
		return tmp;
	}else{
		ErrorDx("BImgBank->GetImg->Not Found:%s", _key);		
		return tmp;
	}
}
