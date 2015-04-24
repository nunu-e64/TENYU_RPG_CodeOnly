////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#ifndef LOGWINDOW_H							////
#define LOGWINDOW_H							////
////////////////////////////////////////////////


class CCmdList;

class CLogWindow{
public:
	CLogWindow();
	~CLogWindow(){
		DESTRUCTED;
	};

	void Init(int _posx, int _posy, int _width, int _height, int _boxColor, int _line , int _word, int _fontSize, int _fontColorMain, int _fontColorSub, int _autoPlaySpeed);
	void Clear();	//�\���e�L�X�g�S����
	void Term();	//Terminate
	
	void Draw();
	
	bool Add(char *_newText);
	bool Add(char **_newTextArray);
	void SetWindowMode(bool _fullMode){
		FullMode = _fullMode;
	}
	bool GetWindowMode(){
		return FullMode;
	}

	void SetVisible(bool _visible){Visible = _visible;}
	
private:
	//�萔
		enum{
			//STOCK_LINE_NUM = 1000,
			LINE_MAX = 255,		//�s���ő�l		�@Init�Œ��������
			WORD_MAX = 255,		//��s�̕������ő�l�@Init�Œ��������
			LINE_SPACE = 5,	//�s��
		};

	//�����o�֐�



	//�����o�ϐ�
		bool Initialized;	//�����ς݂��ۂ�

		char** Text;	//���O���b�Z�[�W�{��

		bool Visible;	//�R�}���h����ύX�\

		/*char OldText[LINE_MAX][WORD_MAX];
		char DrawText[LINE_MAX][WORD_MAX];
*/
		int PosX, PosY, Width, Height;			//����̍��W�A�����c��
		int PosXFull, WidthFull;			//�t���T�C�Y���[�h�̂Ƃ�
		int LineNum, WordNum;	//WordNum�̒P�ʂ̓o�C�g�@LineNum�͍s��
		int FontSize;
		int FontColorMain, FontColorSub;	//Color2�͉e
		int BoxColor;
		int WordWidth;		//WordNum��FontSize�Ɋ�Â��ĉ������Z�o
		bool FullMode;		//���O�E�B���h�E�̑傫����؂�ւ���X�C�b�`

		int NextLine;	//���ɏ�����������ׂ�Text�̔z��C���f�b�N�X

		/*
		int StockLine;		//���s�ڂ܂Ńf�[�^���X�g�b�N����Ă��邩				1~STOCK_LINE_NUM	��F0	//AddText�ŏ���������̂�NoProblem
		int TextLineNum();		//���A���s�ڂ܂ŕ\���p�e�L�X�g�����邩		1~LineNum	��F0
		
		int NowStock;	//���A�X�g�b�N�̉��s�ڂɂ��邩	0~	��F-1	(TextBox�O�ɂ����Ă͎����ׂ�s�͂ǂ���)
		int NowTarget;	//���A�e�L�X�g�{�b�N�X�̉��s�ڂɂ��ď������Ă���̂��@0~	��F-1
		bool PageChange;	//���Ƀy�[�W���Z�b�g���邩�ۂ�

		////�e�L�X�g�̃A�j���[�V�����\���֌W//////////////
			int NewText;		//chText�ɐV���ɒǉ����ꂽ�s�@-1����ω��Ȃ��@0~����ォ�牽�s�ڈȍ~���ǉ����ꂽ���i�󔒂��J�E���g�j
			bool Showing;		//�e�L�X�g�A�j���[�V���������ݐi�s���FTrue�@�����FFalse
			int ShowingTime; 

		////AddText�̂Ƃ��ɕύX
			int ObjCount;
			direction_tag OriginalDir;

		//���r�p
			struct ruby_tag{
				char Word[32];
				char Ruby[32];
				char chNum[3];	//00~99
			};
			std::vector <ruby_tag> Ruby;
			*/
};


////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										////
////////////////////////////////////////////////