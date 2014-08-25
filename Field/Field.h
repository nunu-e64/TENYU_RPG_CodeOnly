////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#ifndef FIELD_H								////
#define FIELD_H								////
////////////////////////////////////////////////

#include "../Define.h"	//�C���N���[�h���Ȃ��Ă������i�Ԑ��͏o�邪�R���p�C�����ɉ������邽�߃G���[�͏o�Ȃ��j

#include "../Main/TextBox.h"
#include "../Main/TextWrap.h"
#include "../Main/CmdManager.h"
#include "../Main/CmdList.h"
#include "Map.h"
#include "EveManager.h"

#include "../Main/WorldManager.h"

class CField : public CWorldManager{
public:
	~CField();
	
	bool Init(playdata_tag* _playdata_p, const int _dnum);
	int MainLoop();
	
	void Draw(bool _screenflip=false, bool _textshowingstop=false, int dx=0, int dy=0, bool _playeralsoshake=false);
	
	bool Walk(int _dir, int _walkspeed=0, bool _eventwalk=false, bool _walk=true, int _fade=0);

	inline int GetNowMap(){return NowMap;}
	inline void SetNowMap(int _mapnum) {	if(_mapnum<MAP_MAX) NowMap = _mapnum;}

	void SetPosition(int _mapnum, int _x, int _y, bool _d=false);
	void SetMyPic(const int _img[CHARA_PIC_NUM], const char* _pickey);
	void SetMyDir(int _dir){Dir = direction_tag(_dir);}
	void SetMyVisible(bool _visible){Visible=_visible;}
	void Jump();

	void ChangeTextMode(bool _box, const char* _eventtext = NULL);
	////////////////////////////////////////////
	CFlagSet FlagSet;



////////////////////////////////////////////////////////////////
private:
	//�����o�֐�
		bool CheckEvent(bool _foot=false, bool _push=false);
		bool StartSet (const int _dnum);
		bool SaveData(int _dnum, const char _dataname[32] = "");
		
	//�����o�ϐ�
		int NowMap;
	
		unsigned int X, Y;
		unsigned int OldX, OldY;
		int ImgPlayer[CHARA_PIC_NUM];	char PlayerPicKey[32];
		enum direction_tag Dir;
		int Step;	//0~3
		int Dx, Dy;
		bool Visible;

		playdata_tag *PlayData_p;	//Main.cpp����Z�[�u�f�[�^�����ׂă|�C���^�ł�����Ă���

		int TextAutoPlaySpeed;

	//�����C���X�^���X
		CMap Map;
		CTextBox* TextBox;
		///�������Ɉڂ���Field.Init�Œ�~���錴���s��14/06/10//////////$
		//CTextBox  TextBox1;
		//CTextWrap TextWrap1;
		///////////////////////////////////////////////////////
		CFieldCmdManager FieldCmdManager;
		CFirstSetCmdManager FirstSetCmdManager;
		CCmdList CmdList;
		CEveManager EveManager;
};


////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										////	
////////////////////////////////////////////////

