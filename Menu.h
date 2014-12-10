////多重インクルード防止（インクルードガード）//	//pragmaonceと同じ意味だがこちらはコンパイラに依存しない
#ifndef MENU_H								////
#define MENU_H								////
////////////////////////////////////////////////

class CMenuNode{	//CMenuの中に定義する？
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


////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////