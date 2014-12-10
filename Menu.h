////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//	//pragmaonce�Ɠ����Ӗ�����������̓R���p�C���Ɉˑ����Ȃ�
#ifndef MENU_H								////
#define MENU_H								////
////////////////////////////////////////////////

class CMenuNode{	//CMenu�̒��ɒ�`����H
public:
	CMenuNode(const char _label[32]){
		CONSTRUCTED;
		strcpy_s(label, _label);
		parent = NULL;
		child  = NULL;
		prev   = NULL;
		next   = NULL;
	}
	~CMenuNode(){
		DESTRUCTED;
	}
	
	char label[32];
	CMenuNode* parent;
	CMenuNode* child;
	CMenuNode* prev;
	CMenuNode* next;
};

class CMenu{
public:	
	CMenu(){
		CONSTRUCTED;
		Cursor = front = NULL;
		Alive = false;
	}
	~CMenu(){
		DESTRUCTED;
		Clear(front);
	};

	void Init(int _x, int _y, int _width, int _height);
	void Create(const char _frontlabel[32]);
	void Add(const char _parentlabel[32], const char _newlabel[32]);
	void Clear(CMenuNode* _top);
	
	virtual void Draw()=0;

	CMenuNode* Find(const char _label[32], CMenuNode* _top = NULL);
	int GetIndex(CMenuNode* _node);
	bool Move(CMenuNode* &_result);

	CMenuNode* front;
	CMenuNode* Cursor;

	bool Alive;

protected:
	int X, Y;
	int Width, Height;

};

class CBattleMenu : public CMenu{
public:
	CBattleMenu(){CONSTRUCTED;}
	~CBattleMenu(){DESTRUCTED;}

	void Draw();
};


////���d�C���N���[�h�h�~�i�C���N���[�h�K�[�h�j//
#endif										////
////////////////////////////////////////////////