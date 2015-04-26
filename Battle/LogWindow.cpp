#include "../Define.h"
#include "LogWindow.h"
#include "BImgBank.h"

CLogWindow::CLogWindow(){
	CONSTRUCTED;

	Visible = false;
	Initialized = false;

	//
	//StockLine = -1;

	//NowStock = 0;
	//NowTarget = 0;
	//PageChange = false;

	//NewText = -1;
	//Showing = false;
	//ShowingTime = -1;

	//OriginalDir = DOWN;
}

void CLogWindow::Init(int _posx, int _posy, int _width, int _height, int _boxColor, int _line , int _word, int _fontSize, int _fontColorMain, int _fontColorSub, CBImgBank* _bImgBank){

	if (Initialized) {
		ERRORDX("Already Initialized (do nothing)");
		return;
	}
	
	LineNum = between(1, (int)LINE_MAX, _line);
	WordNum = between(1, (int)WORD_MAX, _word);

	myLog("new LogWindow.Text[%d][%d]...", LineNum, WordNum);
	Text = new char* [LineNum];
	for (int i=0; i<_line; i++){
		Text[i] = new char[WordNum];	
		Text[i][0] = '\0';
	}


	PosX = _posx;
	PosY = _posy;
	Width = _width;
	Height = _height;
	BoxColor = _boxColor;

	FullMode = false;
	PosXFull = 400;
	WidthFull = WINDOW_WIDTH - PosXFull;

	FontSize = _fontSize;
	FontColorMain = _fontColorMain;
	FontColorSub = _fontColorSub;

	NextLine = 0;
	
	_bImgBank->GetImg(LOG_WINDOW_BUTTON, ButtonImg, ARRAY_SIZE(ButtonImg));

	//NowStock = 0;
	//NowTarget = 0;

	//NewText = -1;
	//Showing = false;


	//AddStock�ł̔��p�����I�[�o�[���������邽�߂ɉ���������t����
		char tmp[WORD_MAX] = "��";	
		int tmpnum = 0;
		while(++tmpnum < WordNum/2){
			sprintf_s(tmp, "%s��", tmp);
		}
		WordWidth = GetDrawStringWidth(tmp, strlen(tmp));

	Visible = true;
	Initialized = true;

}

void CLogWindow::Term(){
	if (Initialized){
		myLog("deleting LogWindow.Text[%d][%d]...", LineNum, WordNum);
		for (int i=0; i<LineNum; i++){
			delete [] Text[i];
		}
		delete [] Text;
	
		Initialized = false;
	}
	/*	
	NowStock = 0;
	NowTarget = 0;

	StockLine = -1;
	StockClear();
		
	Showing = false;
	TextClear();
	Alive = false;
	Visible = false;
	AutoPlay = false;
	AutoPlaySpeed = DefaultAutoPlaySpeed;
*/
}

bool CLogWindow::Add(char *_newText){		//�R�����g�s��󔒍s��Load�̒i�K�Ŕr������Ă���
	
	if (!Initialized) {
		ERRORDX("LogWindow hasn't Initialized yet.(I'll do nothing.)");
		return false;
	}

	mystrcpy(Text[NextLine], _newText); 
	NextLine = (++NextLine) % LineNum;
	
	Visible = true;

	return true;

			/*
			////���p�����Ŗ��ߐs�������Ƃ��Ƀe�L�X�g�{�b�N�X���I�[�o�[����o�O���C��=�����`�掞�̃T�C�Y�Ŕ��f
			}else if (GetDrawStringWidth(String, strlen(String)) > WordWidth){			//�����I�[�o�[������ WordWidth��Init()�Œ�`
				char chOverstring[WORD_MAX];
				char chTruestring[WORD_MAX];
				char tmp[WORD_MAX]="";
				
				int tmpnum=0;				//��s���̉����ŋ�؂邽�߂̈ʒu�T��
				while(GetDrawStringWidth(tmp, strlen(tmp)) <= WordWidth){
					tmp[tmpnum]=String[tmpnum];
					tmp[++tmpnum]='\0';
				}

				if (_ismbblead(String[tmpnum-1])) {
					tmpnum--;		//�s�����S�p������1�o�C�g�ڂ������ꍇ�A������������̂�1�o�C�g���炷	$�����炪���Ă΂����炪����������[��WRAP�������������������H
				}
				strcpy_s(chOverstring, String+tmpnum);
				strncpy_s(chTruestring, String, tmpnum);

				AddStock(chTruestring);
				if (strlen(chOverstring)>0) AddStock(chOverstring);				//�I�[�o�[��������؂����čēx�ǂݍ���

			}else if (mystrlen(String) > WordNum){			//�����I�[�o�[�������i���ۂ͕`��T�C�Y�ŏ�������悤�ɏ�L�ɏ������������߂�������g�����Ƃ͂Ȃ��Ǝv����j
				char chOverstring[WORD_MAX];
				char chTruestring[WORD_MAX];
				
				int d = (_ismbblead(String[WordNum-1])? -1:0);	//�s�����S�p������1�o�C�g�ڂ������ꍇ�A������������̂�1�o�C�g���炷
				strcpy_s(chOverstring, String+WordNum+d);
				strncpy_s(chTruestring, String, WordNum+d);
				
				//ebugDx("Debug->�����I�[�o�[:%s", chOverstring);

				AddStock(chTruestring);
				AddStock(chOverstring);				//�I�[�o�[��������؂����čēx�ǂݍ���
			*/
		
}

bool CLogWindow::Add(char **_newTextArray){
	if (_newTextArray == NULL) return false;
	
	for (int i=0; !mystrcmp(_newTextArray[i], EOP)&&strlen(_newTextArray[i]); i++){
		if (!Add(_newTextArray[i])) return false;
	}
	return true;
}

void CLogWindow::Draw(){

	if(!Visible) return;
	
	if (CheckHitKeyDown(KEY_INPUT_P)) SetWindowMode(!FullMode);

	int oldFontSize = GetFontSize();
	
	//�{�b�N�X
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (FullMode?200:100));
		DrawBox((!FullMode?PosX:PosXFull), PosY,     (!FullMode?PosX:PosXFull)+(!FullMode?Width:WidthFull), PosY+Height, BoxColor, true);
		DrawBox((!FullMode?PosX:PosXFull)+5, PosY+5, (!FullMode?PosX:PosXFull)+(!FullMode?Width:WidthFull)-5, PosY+Height-5, GRAY, false);
	
	//�{�^��
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (FullMode?150:100));
		//DrawRectGraph((!FullMode?PosX:PosXFull)-(int)(ButtonImgSize.x/2), (int)(PosY+Height-ButtonImgSize.y)/2, (int)(!FullMode?0:ButtonImgSize.x), 0, (int)ButtonImgSize.x, (int)ButtonImgSize.y, ButtonImg, true, false);
		DrawCenterGraph((!FullMode?PosX:PosXFull), (PosY+Height)/2, ButtonImg[(!FullMode?0:1)], true);

	//�e�L�X�g
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (FullMode?220:200));

		//�e�L�X�g�`��
		SetFontSize(FontSize);
		int line;
		int skipCount = 0;	//��s�͏�l��
		char* textForDraw = new char[(FullMode? WordNum: 8)];
		for (int i = 0; i+skipCount < LineNum; i++){
			line = mod ((i + NextLine + skipCount), LineNum);
			if (strlen(Text[line])) {
				mystrcpy(textForDraw, Text[line], (FullMode? WordNum: 5));
				if (!FullMode) sprintf_s(textForDraw, -1, "%s..", textForDraw);
				DrawString((!FullMode?PosX:PosXFull)+5, PosY + (FontSize+LINE_SPACE)*i+1, textForDraw, FontColorSub);
				DrawString((!FullMode?PosX:PosXFull)+5, PosY + (FontSize+LINE_SPACE)*i  , textForDraw, FontColorMain);
				//DrawString((!FullMode?PosX:PosXFull)+(!FullMode?Width:WidthFull)/2 - WordNum*(FontSize+1)/4 + 1, PosY+Height/2 + LINE_SPACE/2 - LineNum*(FontSize+LINE_SPACE)/2 + (FontSize+LINE_SPACE)*line+1, Text[line], FontColorSub);
				//DrawString((!FullMode?PosX:PosXFull)+(!FullMode?Width:WidthFull)/2 - WordNum*(FontSize+1)/4	, PosY+Height/2 + LINE_SPACE/2 - LineNum*(FontSize+LINE_SPACE)/2 + (FontSize+LINE_SPACE)*line	, Text[line], FontColorMain);
			} else {
				++skipCount;
				--i;
			}
		}

		delete [] textForDraw;
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND,-1);
		SetFontSize(oldFontSize);
}

void CLogWindow::Clear(){

	for (int i=0; i<LineNum; i++){
		Text[i][0] = '\0';
	}
	NextLine = 0;
	FullMode = false;
	Visible = true;
}

/*void CTextBox::Draw_Animation(bool _showingstop){
	
	if (Showing){	//�e�L�X�g�A�j���[�V������
		if (!_showingstop) ShowingTime++;
	}else if (NewText != -1 && !Showing){	//�e�L�X�g�A�j���[�V�����O
		ShowingTime = 0;
		Showing = true;
	}else if (NewText == -1 && !Showing){	//�e�L�X�g�A�j���[�V�����ς�
		for (int i = 0; i<LineNum; i++){
			strcpy_s(chDrawText[i], chText[i]);
		}
	}			
			
	if (Showing && !_showingstop){
		int NowShow = (int)(SHOWING_SPEED*ShowingTime/60)*2;
		int Sumstrlen = 0;

		for (int i = 0; i<NewText; i++){
			strcpy_s(chDrawText[i], chText[i]);
		}

		for (int i = NewText; i<LineNum; i++){
			if (NowShow-Sumstrlen <= 0) {
				strcpy_s(chDrawText[i], "");
			}else{
				strncpy_s(chDrawText[i], chText[i], min(NowShow-Sumstrlen, mystrlen(chText[i])));	//���r�p@�͕\�����Ȃ��̂ł����ŃA�j���[�V����������Ȃ��悤�ɂ���R�[�h�i�܂������ăi�C�j��//min(NowShow-Sumstrlen+(chText[i][min(NowShow-Sumstrlen, mystrlen(chText[i]))]=='@'?1:0), mystrlen(chText[i]))	$
			}
			Sumstrlen += strlen(chText[i]);
		} 
		
		if (NowShow > Sumstrlen) {
			Showing = false;
			NewText = -1;
		}
	}
}
*/