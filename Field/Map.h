////多重インクルード防止（インクルードガード）//
#ifndef MAP_H								////
#define MAP_H								////
////////////////////////////////////////////////


class CMap{
public:
	CMap(){
		ImgData.clear();
		for (int i=0; i<CHARA_PIC_NUM; i++){
			dammyimg.Img[i]=NULL;
		}
		ImgData.push_back(dammyimg);

		for (int m=0; m<MAP_MAX; m++){
			MapGraph[m] = -1;
			MapCreated[m] = false;
		}
	};
	~CMap(){
		DebugDx("CMap::~CMap()");
	}
	
	void Init(){ImgData.clear();}

	void Draw(int _mapnum, int _x, int _y, int _dx=0, int _dy=0);

	void SetMap(unsigned int _mapnum, int _filesize, unsigned char* buf);
	void LoadChip(const char *_path, int _mapnum, bool _mapchip=true);
	void LoadPic(const char *_path, const char _key[32], const char _kind[32]=NULL);
	
	void CreateMapGraph(int _mapnum = -1);

	int GetMapData(int _mapnum, int _x, int _y, int layer=0);
	int* GetImgData(const char _key[32]);

private:
	unsigned char BasMap[MAP_MAX][MAP_SIZE][MAP_SIZE];	//[x][y]
	unsigned char ObjMap[MAP_MAX][MAP_SIZE][MAP_SIZE];	//[x][y]

	int ImgMapChip[MAP_MAX][256];
	int ImgObjChip[MAP_MAX][256];
	
	int MapGraph[MAP_MAX];		//マップ全体のグラフィックを事前準備しDrawではDrawRectGraphだけで描画
	bool MapCreated[MAP_MAX];	//マップグラフィックの事前準備ができているか	

	//@Load_Pic用	//vectorではなくmapにしたほうが早そう$
		struct ImgData_tag{	
			int	Img[CHARA_PIC_NUM];
			char Key[32];
			char Kind[32];		//ぶっちゃけNPCかどうかしか問題ではない
		};
		std::vector<ImgData_tag> ImgData;
		ImgData_tag dammyimg;
	
};



////多重インクルード防止（インクルードガード）//
#endif										////	
////////////////////////////////////////////////