////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#ifndef IMGBANK_H						////
#define IMGBANK_H						////
////////////////////////////////////////////////

#include <map>
#include <string>
#include <vector>

class CImgBank{
public:
	CImgBank(){
		CONSTRUCTED;
	}
	~CImgBank(){
		DESTRUCTED;
		ImgBank.clear();
	}

	void LoadPic(const char *_path, const char _key[32], const char _kind[32]);	//CMap�Ƃ̌݊����m��
	bool AddImg(const char* _key, const int _img);
	int GetImg(const char* _key);

private:
    std::map <std::string, int> ImgBank;

};


////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										////
////////////////////////////////////////////////