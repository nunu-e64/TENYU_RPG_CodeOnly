////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#ifndef SCREENCHANGER_H					////
#define SCREENCHANGER_H					////
////////////////////////////////////////////////

class CScreenChanger{
public:
	enum screenchange_tag{
		SCREEN_FADE,
		SCREEN_BOKASHI,
		SCREEN_NUM
	};

	void ChangeScreen(const int _pGraph, const int _nGraph, const screenchange_tag _type, int _count);


private:
	static void Fade(const int _pGraph, const int _nGraph, int _count);
	static void Bokashi(const int _pGraph, const int _nGraph, int _count);
};


////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										////
////////////////////////////////////////////////