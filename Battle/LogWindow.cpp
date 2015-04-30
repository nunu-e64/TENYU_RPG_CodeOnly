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

void CLogWindow::Init(int _smallposx, int _posy, int _smallwidth, int _height, int _boxColor, int _stockLine, int _fontSize, int _fontColorMain, int _fontColorSub, CBImgBank* _bImgBank){

	if (Initialized) {
		ERRORDX("Already Initialized (do nothing)");
		return;
	}
	

	PosX = _smallposx;
	PosY = _posy;
	Width = _smallwidth;
	Height = _height;
	BoxColor = _boxColor;

	FullMode = false;
	PosXFull = 400;
	WidthFull = WINDOW_WIDTH - PosXFull;

	FontSize = _fontSize;
	FontHandle = CreateFontToHandle(NULL, FontSize, -1);
	FontColorMain = _fontColorMain;
	FontColorSub = _fontColorSub;
	_bImgBank->GetImg(LOG_WINDOW_BUTTON, ButtonImg, ARRAY_SIZE(ButtonImg));

	NextLine = 0;
	BackLine = 0;
	
	//Width���當�����v�Z->WordNum
		WordWidth = WidthFull - YOHAKU*2;
		char tmp[WORD_MAX] = "";
		WordNum = WORD_MAX;
		for (int i=0; i<WORD_MAX-1; i++){
			tmp[i]	 = 'a';
			tmp[i+1] = '\0';
			if (GetDrawStringWidthToHandle(tmp, strlen(tmp), FontHandle) > WordWidth){
				WordNum = i+2;
				break;
			}
		}

	//Height��FontSize����s���v�Z->LineNum
		StockLineNum = max(1, _stockLine);
		LineNum = min((Height-YOHAKU*2)/(FontSize+LINE_SPACE), StockLineNum);

	myLog("new LogWindow.Text[%d][%d]...", StockLineNum, WordNum);
	Text = new char* [StockLineNum];
	for (int i=0; i<StockLineNum; i++){
		Text[i] = new char[WordNum];	
		Text[i][0] = '\0';
	}


	//NowStock = 0;
	//NowTarget = 0;

	//NewText = -1;
	//Showing = false;

	Visible = true;
	Initialized = true;

}

void CLogWindow::Clear(){

	for (int i=0; i<StockLineNum; i++){
		Text[i][0] = '\0';
	}
	NextLine = 0;
	FullMode = false;
	Visible = true;
}

void CLogWindow::Term(){
	if (Initialized){
		myLog("deleting LogWindow.Text[%d][%d]...", StockLineNum, WordNum);
		for (int i=0; i<StockLineNum; i++){
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

	////�����`�掞�̃T�C�Y�ƕ������̗����ŉ��s�̗v�s�v���f
	if (GetDrawStringWidthToHandle(_newText, strlen(_newText), FontHandle) > WordWidth){			//�����I�[�o�[������ WordWidth��Init()�Œ�`
		char chOverstring[WORD_MAX];
		char chTruestring[WORD_MAX];
		char tmp[WORD_MAX] = "";
				
		int tmpnum=0;				//��s���̉����ŋ�؂邽�߂̈ʒu�T���@tmpnum=\0���܂܂Ȃ��K��������
		while(GetDrawStringWidthToHandle(tmp, strlen(tmp), FontHandle) <= WordWidth){
			tmp[tmpnum] = _newText[tmpnum];
			tmp[++tmpnum] = '\0';
		}
		--tmpnum;

		if (_ismbblead(_newText[tmpnum-1])) {
			--tmpnum;		//�s�����S�p������1�o�C�g�ڂ������ꍇ�A������������̂�1�o�C�g���炷
		}
				
		strncpy_s(chTruestring, _newText, tmpnum);
		Add(chTruestring);
		strcpy_s(chOverstring, _newText+tmpnum);
		Add(chOverstring);				//�I�[�o�[��������؂����čēx�ǂݍ���

	}else if (mystrlen(_newText) > WordNum){			//�����I�[�o�[�������i���ۂ͕`��T�C�Y�ŏ�������悤�ɏ�L�ɏ������������߂�������g�����Ƃ͂Ȃ��Ǝv����j
		char chOverstring[WORD_MAX];
		char chTruestring[WORD_MAX];
		
		int d = (_ismbblead(_newText[WordNum-1])? -1:0);	//�s�����S�p������1�o�C�g�ڂ������ꍇ�A������������̂�1�o�C�g���炷
		strncpy_s(chTruestring, _newText, WordNum+d);
		Add(chTruestring);
		strcpy_s( chOverstring, _newText+WordNum+d);
		Add(chOverstring);	

	} else {

		mystrcpy(Text[NextLine], _newText); 
		NextLine = (++NextLine) % StockLineNum;
	
		Visible = true;

	}
	
	return true;
}

bool CLogWindow::Add(char **_newTextArray){
	if (_newTextArray == NULL) return false;
	
	for (int i=0; !mystrcmp(_newTextArray[i], EOP)&&strlen(_newTextArray[i]); i++){
		if (!Add(_newTextArray[i])) return false;
	}
	return true;
}

bool CLogWindow::Main(){

	if (FullMode) {
		if (CheckHitKey(KEY_INPUT_UP)) {
			BackLine = min(BackLine+1, StockLineNum-LineNum);
		} else if (CheckHitKey(KEY_INPUT_DOWN)) {
			BackLine = max(BackLine-1, 0);
		}
		if (CheckHitKeyDown(KEY_INPUT_CANCEL)) FullMode = false;
	}

	return FullMode;
}

void CLogWindow::Draw(){

	if(!Visible) return;

	if (CheckHitKeyDown(KEY_INPUT_P)) {	//�E�B���h�E���[�h�ؑցi�U���G�t�F�N�g���ɂ����O�m�F�ł���悤�AMain�ł͂Ȃ�Draw�ŃL�[�`�F�b�N�j
		SetWindowMode(!FullMode);
		BackLine = 0;
	}

	//�{�b�N�X
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (FullMode?200:100));
		DrawBox((!FullMode?PosX:PosXFull),		  PosY,	       (!FullMode?PosX:PosXFull)+(!FullMode?Width:WidthFull), PosY+Height, BoxColor, true);
		DrawBox((!FullMode?PosX:PosXFull)+YOHAKU, PosY+YOHAKU, (!FullMode?PosX:PosXFull)+(!FullMode?Width:WidthFull)-5, PosY+Height-5, GRAY, false);
	
	//�{�^��
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (FullMode?150:100));
		DrawCenterGraph((!FullMode?PosX:PosXFull), (PosY+Height)/2, ButtonImg[(!FullMode?0:1)], true);

	//�e�L�X�g
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, (FullMode?220:200));

		//�e�L�X�g�`��
		int line;
		int skipCount = 0;	//��s�͏�l��
		int drawCount = 0;  //���ۂɕ`�悵���s�̐�
		char* textForDraw = new char[(FullMode? WordNum: 8)];
		for (int i = 0; i+skipCount < StockLineNum && drawCount < LineNum; i++){
			line = mod ((NextLine - LineNum - BackLine + i + skipCount), StockLineNum);
			if (strlen(Text[line])) {
				mystrcpy(textForDraw, Text[line], (FullMode? WordNum: 5));
				if (!FullMode) sprintf_s(textForDraw, -1, "%s..", textForDraw);
				DrawStringToHandle((!FullMode?PosX:PosXFull)+YOHAKU, PosY + (FontSize+LINE_SPACE)*i+1+YOHAKU, textForDraw, FontColorSub , FontHandle);
				DrawStringToHandle((!FullMode?PosX:PosXFull)+YOHAKU, PosY + (FontSize+LINE_SPACE)*i+YOHAKU  , textForDraw, FontColorMain, FontHandle);
				//DrawString((!FullMode?PosX:PosXFull)+(!FullMode?Width:WidthFull)/2 - WordNum*(FontSize+1)/4 + 1, PosY+Height/2 + LINE_SPACE/2 - LineNum*(FontSize+LINE_SPACE)/2 + (FontSize+LINE_SPACE)*line+1, Text[line], FontColorSub);
				//DrawString((!FullMode?PosX:PosXFull)+(!FullMode?Width:WidthFull)/2 - WordNum*(FontSize+1)/4	, PosY+Height/2 + LINE_SPACE/2 - LineNum*(FontSize+LINE_SPACE)/2 + (FontSize+LINE_SPACE)*line	, Text[line], FontColorMain);
				++drawCount;
			} else {
				++skipCount;
				--i;
			}
		}

		delete [] textForDraw;
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND,-1);
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