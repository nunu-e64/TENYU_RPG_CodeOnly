
#include "Define.h"
#include "Main/Main.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	#ifdef MEMORY_CHECK
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);  //new��delete���Ă��Ȃ��������o
	#endif

	
	SetMainWindowText(GAME_TITLE);
	ChangeWindowMode( TRUE );			// �E�C���h�E���[�h�ɕύX
	
	if( DxLib_Init() == -1 ) return -1 ; // �c�w���C�u����������
	
	SetDrawScreen( DX_SCREEN_BACK ); //�`������ʗ���
	SetTransColor(0, 0, 0);	//���ߐF�w��
	SetMouseDispFlag( false );	//�}�E�X�J�[�\����\�����Ȃ�
		
	myLogf("MyLog_Printed", "--------START------------------------------------------------------\n");
	myLog("--------START------------------------------------------------------\n");

		CMain* Main = CMain::GetInstance();		//����ȉ������ׂ�CMain::Main�Ƃ����ÓI�����o�֐��ɂ���̂�����
		if (Main->Init()){
			while( BasicLoop() ){
		
				if(!Main->GameLoop()) break;
			
			}
		}

	DxLib_End(); // DX���C�u�����I������

	myLogf("MyLog_Printed", "----FINISH-----------------------------------------------------");	
	myLog("----FINISH-----------------------------------------------------");

    return 0;
}  

direction_tag sys::TurnDir(int _dir, signed int _rightspin){		

	int _turneddir;

	switch (_dir){
	case UP:
		_turneddir = 0;		break;
	case RIGHT:
		_turneddir = 1;		break;
	case DOWN:
		_turneddir = 2;		break;
	case LEFT:
		_turneddir = 3;		break;
	default:
		_turneddir = 0;		break;
	}
	
	_turneddir += _rightspin;
	_turneddir%=4;	//-3~3
	_turneddir+=4;	//1~7
	_turneddir%=4;	//0~3

	return (direction_tag) choose(_turneddir+1, UP, RIGHT, DOWN, LEFT);
}

direction_tag sys::StrtoDir(const char* _str, int _originaldir){	//CCmdManager��private�֐��ɂ��Ă���������
	int dir;
	int spin = 0;
	char* p;
	char* str = new char[strlen(_str)+1];
	mystrcpy(str, _str);

	if ((p=strchr(str, '+'))!=NULL){
		mystrtol(p, &spin);
		*p = '\0';
	}else if ((p=strchr(str, '-'))!=NULL){
		mystrtol(p, &spin);
		*p = '\0';
	}

	if( mystrcmp(str, 'p', 6, "Right","RIGHT","right","��","0","�E") ){
		dir = RIGHT;
	}else if( mystrcmp(str, 'p', 6, "Left","LEFT","left","��","1","��") ){
		dir = LEFT;
	}else if( mystrcmp(str, 'p', 6, "Down","DOWN","down","��","2","��") ){
		dir = DOWN;
	}else if( mystrcmp(str, 'p', 6, "Up","UP","up","��","3","��") ){
		dir = UP;
	}else if( mystrcmp(str, 'p', 4, "Original", "ORIGINAL","original","-1") ){
		dir = (direction_tag)_originaldir;
	}else{
		WarningDx("Warning->StrtoDir failed:%s", __FILE__, __LINE__, str);
		dir = DOWN;
	}

	delete [] str;
	return TurnDir(dir, spin);
}

bool sys::PlayerName(const char* _str){
	return mystrcmp(_str, 'p', 6, "me", "ME", "Me", "PLAYER", "player", "Player");
}

bool sys::TrueOrFalse(const char* _str, bool _torf){
	if (_torf){
		return mystrcmp(_str, 'p', 4, "1", "TRUE", "True", "true");
	}else{
		return mystrcmp(_str, 'p', 4, "0", "FALSE", "False", "false");
	}
}

int sys::rank3(const char* _str, int _exception) {
	if (mystrcmp(_str, 'p', 3, "LOW", "low", "Low")) {
		return 1;
	} else if (_str==NULL || mystrcmp(_str, 'p', 3, "MIDDLE", "middle", "Middle")) {
		return 2;
	} else if (mystrcmp(_str, 'p', 3, "HIGH", "high", "High")) {
		return 3;
	} else {
		WarningDx("Warning->Check _str[low,middle,high]:%s", _str);
		return _exception;
	}
}

bool sys::CheckStrNULL(const char* _str){
	if (mystrcmp(_str, 'p', 3, "NULL", "null", "Null")) {
		return true;
	} else {
		return false;
	}
}

bool sys::CheckStrNULL(const std::string _str){
	if (_str=="NULL" || _str=="null" || _str=="Null") {
		return true;
	} else {
		return false;
	}
}
