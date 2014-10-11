////多重インクルード防止（インクルードガード）//
#ifndef SCREENCHANGER_H					////
#define SCREENCHANGER_H					////
////////////////////////////////////////////////

class CScreenChanger{
public:
	void ChangeScreen(const int _pGraph, const int _nGraph, const screenchange_tag _type, const int _count);

private:
	static void Fade(const int _pGraph, const int _nGraph, const int _count);
};



////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////