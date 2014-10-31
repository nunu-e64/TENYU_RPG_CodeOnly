////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#ifndef BIMGBANK_H						////
#define BIMGBANK_H						////
////////////////////////////////////////////////

#include <map>
#include <string>
#include <vector>

class CBImgBank{
public:
	~CBImgBank(){
		ImgBank.clear();
		for (int i=0; i<MAP_MAX; i++){
			BattleBackGround[i].clear();
		}
	}

	void LoadPic(const char *_path, const char _key[32], const char _kind[32]);	//CMap�Ƃ̌݊����m��
	bool Add(const char* _key, const int _img);
	void Init();

	int GetImg(const char* _key);
	int GetBattleBackGround(int _mapnum, int _chipnum=-1);
	void SetBattleBackGround(const char* _key,int _mapnum, int _chipnum=-1);

private:
    std::map <std::string, int> ImgBank;
		//���݂��Ȃ��L�[��n�����Ƃ��Ƀf�t�H���g�R���X�g���N�^�ŏ���ɍ��map�̎d�l�͂�͂�g���ɂ����B�p���Ŏ����p�ɃJ�X�^�}�C�Y�ł���Ȃ炷�ׂ��B$

	struct bbg_tag{
		int chipnum;
		int gragh;
	};
	std::vector <bbg_tag> BattleBackGround[MAP_MAX];
		//map�Ŏ�������Ƒ��݂��Ȃ��L�[[chipnum]�Ō����������Ƃ��Ɏ����������Ăǂ�ǂ񃁃�����H���Ă��܂��̂ŁA�\���̂Ŏ���
};


////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										////
////////////////////////////////////////////////