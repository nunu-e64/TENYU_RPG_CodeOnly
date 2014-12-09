////多重インクルード防止（インクルードガード）//
#ifndef BIMGBANK_H						////
#define BIMGBANK_H						////
////////////////////////////////////////////////

#include <map>
#include <string>
#include <vector>

class CBImgBank{
public:
	CBImgBank(){
		CONSTRUCTED
	}
	~CBImgBank(){
		DESTRUCTED
		ImgBank.clear();
		for (int i=0; i<MAP_MAX; i++){
			BattleBackGround[i].clear();
		}
	}

	void LoadPic(const char *_path, const char _key[32], const char _kind[32]);	//CMapとの互換性確保
	bool Add(const char* _key, const int _img);
	void Init();

	int GetImg(const char* _key);
	void SetBattleBackGround(const char* _key,int _mapnum, int _chipnum=-1);
	int GetBattleBackGround(int _mapnum, int _chipnum=-1);

private:
    std::map <std::string, int> ImgBank;

	struct bbg_tag{
		int chipnum;
		int gragh;
	};
	std::vector <bbg_tag> BattleBackGround[MAP_MAX];
		//mapで実装すると存在しないキー[chipnum]で検索かけたときに自動生成してどんどんメモリを食ってしまうので、構造体で実装→find使えばOK。mapに戻そう$
};


////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////