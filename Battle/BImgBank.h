////多重インクルード防止（インクルードガード）//
#ifndef BIMGBANK_H						////
#define BIMGBANK_H						////
////////////////////////////////////////////////

#include <map>
#include <string>

class CBImgBank{
public:

	void LoadPic(const char *_path, const char _key[32], const char _kind[32]);	//CMapとの互換性確保
	bool Add(const char* _key, const int _img);
	void Clear(){ImgBank.clear();}

	int GetImg(const char* _key);

private:
    std::map <std::string, int> ImgBank;
		//存在しないキーを渡したときにデフォルトコンストラクタで勝手に作るmapの仕様はやはり使いにくい。継承で自分用にカスタマイズできるならすべし。$
};


////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////