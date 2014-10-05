////多重インクルード防止（インクルードガード）//
#ifndef BIMGBANK_H						////
#define BIMGBANK_H						////
////////////////////////////////////////////////

#include <map>
#include <string>

class CBImgBank{
public:
	//
	//static CBImgBank* GetInstance(){
 //   	static CBImgBank Instance;  // 唯一のインスタンス（静的メンバ関数･･･インスタンス生成前から存在しておりCMain::GetInstanceの形でどこからでも呼び出せる）
 //   	return &Instance;
 //   }

	void LoadPic(const char *_path, const char _key[32], const char _kind[32]);	//CMapとの互換性確保
	bool Add(const char* _key, const int _img);
	void Clear(){ImgBank.clear();}

	int GetImg(const char* _key);

private:/*
    CBImgBank(){}
    CBImgBank(const CBImgBank& rhs);
    CBImgBank& operator=(const CBImgBank& rhs);
*/
	std::map <std::string, int> ImgBank;
		//存在しないキーを渡したときにデフォルトコンストラクタで勝手に作るmapの仕様はやはり使いにくい。継承で自分用にカスタマイズできるならすべし。$
};


////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////