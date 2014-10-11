#include "../Define.h"
#include "ScreenChanger.h"



void CScreenChanger::ChangeScreen(const int _pGraph, const int _nGraph, const screenchange_tag _type, int _count){

	static void (*FuncArray[SCREEN_NUM])(const int, const int, int) = {
		&Fade,
		&Bokashi
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


void CScreenChanger::Fade(const int _pGraph, const int _nGraph, int _count){
	int dGraph = MakeScreen( WINDOW_WIDTH, WINDOW_HEIGHT, FALSE ) ;
	int i = 0;
	
	do{
		GraphBlendBlt( _pGraph, _nGraph, dGraph, (int)(255*i/(double)_count), DX_GRAPH_BLEND_NORMAL);
		DrawGraph(0,0,dGraph,false);
	}while(BasicLoop() && ++i<_count);

}

void CScreenChanger::Bokashi(const int _pGraph, const int _nGraph, int _count){
	int dGraph = MakeScreen( WINDOW_WIDTH, WINDOW_HEIGHT, FALSE ) ;
	int dGraph2 = MakeScreen( WINDOW_WIDTH, WINDOW_HEIGHT, FALSE ) ;
	int i = 0;
	
	if (_count%2==1) ++_count;
	double per = 1;

	do{
		//per = i/(double)(_count/2);
		per = sin((PI/2)*i/(double)(_count/2));
		dGraph = MakeScreen( WINDOW_WIDTH-(int)((WINDOW_WIDTH-4)*per), WINDOW_HEIGHT-(int)((WINDOW_HEIGHT-4)*per), FALSE ) ;
		SetDrawScreen(dGraph);
		DrawExtendGraph(0,0,WINDOW_WIDTH-(int)((WINDOW_WIDTH-4)*per),WINDOW_HEIGHT-(int)((WINDOW_HEIGHT-4)*per),_pGraph, false); 
		SetDrawScreen(DX_SCREEN_BACK);
		DrawExtendGraph(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, dGraph,false);
	}while(BasicLoop() && ++i<_count/2);

	i = 0;

	do{
		//per = 1 - i/(double)(_count/2);
		per = sin((PI/2) + (PI/2)*i/(double)(_count/2));
		dGraph = MakeScreen( WINDOW_WIDTH-(int)((WINDOW_WIDTH-4)*per), WINDOW_HEIGHT-(int)((WINDOW_HEIGHT-4)*per), FALSE ) ;
		SetDrawScreen(dGraph);
		DrawExtendGraph(0,0,WINDOW_WIDTH-(int)((WINDOW_WIDTH-4)*per),WINDOW_HEIGHT-(int)((WINDOW_HEIGHT-4)*per), _nGraph, false); 
		SetDrawScreen(DX_SCREEN_BACK);
		DrawExtendGraph(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, dGraph,false);
	}while(BasicLoop() && ++i<_count/2);

	/*do{
		dGraph = _pGraph;
		GraphFilter( dGraph, DX_GRAPH_FILTER_GAUSS, 8, 100*i/(double)(_count/2) ) ;
		DrawGraph(0,0,dGraph,false);
	}while(BasicLoop() && ++i<_count/2);

	i = 0;

	do{
		dGraph2 = _nGraph;
		GraphFilter( dGraph2, DX_GRAPH_FILTER_GAUSS, 8, (int)(100-100*i/(double)(_count/2)) ) ;
		DrawGraph(0,0,dGraph2,false);
		DebugDx("%d",(int)(100-100*i/(double)(_count/2)));
	}while(BasicLoop() && ++i<_count/2);
*/

}
