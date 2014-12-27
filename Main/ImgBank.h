////多重インクルード防止（インクルードガード）//
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

	void LoadPic(const char *_path, const char _key[32], const char _kind[32]);	//CMapとの互換性確保
	bool AddImg(const char* _key, const int _img);
	int GetImg(const char* _key);

private:
    std::map <std::string, int> ImgBank;

};


////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////