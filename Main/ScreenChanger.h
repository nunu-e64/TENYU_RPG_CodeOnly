////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#ifndef SCREENCHANGER_H					////
#define SCREENCHANGER_H					////
////////////////////////////////////////////////

class CScreenChanger{
public:
	void ChangeScreen(const int _pGraph, const int _nGraph, const screenchange_tag _type, const int _count);

private:
	static void Fade(const int _pGraph, const int _nGraph, const int _count);
};



////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										////
////////////////////////////////////////////////