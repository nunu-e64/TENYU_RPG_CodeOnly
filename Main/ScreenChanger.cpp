#include "../Define.h"
#include "ScreenChanger.h"



void CScreenChanger::ChangeScreen(const int _pGraph, const int _nGraph, const screenchange_tag _type, const int _count){

	static void (*FuncArray[SCREEN_NUM])(const int, const int, const int) = {
		&Fade
	};
	//void (*FuncArray[SCREEN_NUM])(const int, const int, const int) = {&Fade};

	FuncArray[_type](_pGraph, _nGraph, _count);

	/*
	switch(_type){
	case FADE:
		Fade(_pGraph, _nGraph, _count);
		break;
	default:
		ErrorDx("Error->CScreenChanger::ChangeScreen->TypeError");
		return;
	}*/
}


void CScreenChanger::Fade(const int _pGraph, const int _nGraph, const int _count){
	int dGraph = MakeScreen( WINDOW_WIDTH, WINDOW_HEIGHT, FALSE ) ;
	int i = 0;
	
	while(BasicLoop() && ++i<_count){
		GraphBlendBlt( _pGraph, _nGraph, dGraph, 255*i/(double)_count, DX_GRAPH_BLEND_NORMAL);
		DrawGraph(0,0,dGraph,false);
	}
};