////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#ifndef BIMGBANK_H						////
#define BIMGBANK_H						////
////////////////////////////////////////////////

#include <map>
#include <string>

class CBImgBank{
public:
	//
	//static CBImgBank* GetInstance(){
 //   	static CBImgBank Instance;  // �B��̃C���X�^���X�i�ÓI�����o�֐�����C���X�^���X�����O���瑶�݂��Ă���CMain::GetInstance�̌`�łǂ�����ł��Ăяo����j
 //   	return &Instance;
 //   }

	void LoadPic(const char *_path, const char _key[32], const char _kind[32]);	//CMap�Ƃ̌݊����m��
	bool Add(const char* _key, const int _img);
	void Clear(){ImgBank.clear();}

	int GetImg(const char* _key);

private:/*
    CBImgBank(){}
    CBImgBank(const CBImgBank& rhs);
    CBImgBank& operator=(const CBImgBank& rhs);
*/
	std::map <std::string, int> ImgBank;
		//���݂��Ȃ��L�[��n�����Ƃ��Ƀf�t�H���g�R���X�g���N�^�ŏ���ɍ��map�̎d�l�͂�͂�g���ɂ����B�p���Ŏ����p�ɃJ�X�^�}�C�Y�ł���Ȃ炷�ׂ��B$
};


////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										////
////////////////////////////////////////////////