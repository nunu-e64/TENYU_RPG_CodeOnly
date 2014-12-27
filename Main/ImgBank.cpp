#include "../Define.h"
#include "ImgBank.h"


void CImgBank::LoadPic(const char *_path, const char _key[32], const char _kind[32]){
	AddImg(_key, LoadGraph(_path, true));
}

bool CImgBank::AddImg(const char* _key, const int _img){
	
	if (mystrcmp2(_key, "null")){
		ERRORDX("You can't use \"null\" for PicKey:%s->(cancel)", _key);
		return false;
	}
	
	if (ImgBank[_key]!=0){
		ERRORDX("AlreadyResiteredImageKey (cancel):%s", _key);
		return false;
	}

	ImgBank[_key] = _img;		//��L�d���`�F�b�Nif���Ŏ���insert���Ă��܂��Ă��邽�߁A������insert����Ɠ����G���[�������������N����Ȃ��Ȃ��Ă��܂�
	return true;
}

int CImgBank::GetImg(const char* _key){
	if (mystrcmp2(_key, "null")) return 0;

	int tmp = ImgBank[_key];
	if (tmp != 0){
		return tmp;
	}else{
		ERRORDX("Not Found:%s", _key);		
		return 0;
	}
}
