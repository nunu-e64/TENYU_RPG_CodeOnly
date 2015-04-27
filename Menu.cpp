#include "Define.h"
#include "Menu.h"

void CMenu::Init(int _x, int _y, int _width, int _height){
	X = _x;
	Y = _y;
	Width = _width;
	Height = _height;
}

void CMenu::Create(const char _frontlabel[32]){

	Clear();
	god = new CMenuNode("NULL");
	god->prev = god;
	god->next = god;

	front = new CMenuNode(_frontlabel);
	front->prev = front;
	front->next = front;
	front->parent = god;
	god->child = front;

	Cursor = front;
	Alive = false;
}

void CMenu::Add(const char _parentlabel[32], const char _newlabel[32]){
	CMenuNode* tmp;

	if (Find(_newlabel)!=NULL){
		ErrorDx("Error->CMenu::Add->Already existed name:%s", _newlabel);
		return;
	}

	if (!(strlen(_parentlabel))){
		tmp = god;
	}else{
		tmp = Find(_parentlabel);
	}
	
	if (tmp == NULL){
		ERRORDX("CMenu::Add->NotFoundParentLabel:%s",_parentlabel); 
	}else if (tmp->child == NULL){
		tmp->child = new CMenuNode(_newlabel);
		tmp->child->parent = tmp;
		tmp->child->next = tmp->child;
		tmp->child->prev = tmp->child;
	}else{
		tmp = tmp->child;
		while (tmp->next != tmp->parent->child){ tmp = tmp->next;}
		tmp->next = new CMenuNode(_newlabel);
		tmp->next->prev = tmp;	
		tmp->next->next = tmp->parent->child;
		tmp->next->parent = tmp->parent;
		tmp->parent->child->prev = tmp->next;
	}
}

void CMenu::Clear(){
	Clear(god);
	Cursor = front = god = NULL;
}
void CMenu::Clear(CMenuNode* _top){
	CMenuNode* tmp = _top;

	if (tmp==NULL) return;

	if (tmp->parent!=NULL) tmp->parent->child=NULL;
	
	_top->prev->next = NULL;	//リングを切る

	while (tmp != NULL){
		if (tmp->child!=NULL) Clear(tmp->child);
		CMenuNode* deleteNode = tmp;
		
		//tmp->prev->next = tmp->next;	消した後にリンクをつなぎなおしたいとき（ノードの一部削除のときに使う）
		//tmp->next->prev = tmp->prev;	消した後にリンクをつなぎなおしたいとき（ノードの一部削除のときに使う）
		
		tmp = tmp->next;
		delete deleteNode;
	}
}

CMenuNode* CMenu::Find(const char _label[32]){
	return Find(_label, front);
}
CMenuNode* CMenu::Find(const char _label[32], CMenuNode* _top){
	//再帰による全探索。見渡してから潜るため浅いほうが概ね発見が早い。
	
	CMenuNode* tmp = _top;

	do{
		if (mystrcmp(tmp->label, _label)) return tmp;
		tmp = tmp->next;
	}while(tmp != _top);

	CMenuNode* result;	//tmp = _top

	do{	
		if (tmp->child!=NULL){
			if ((result=Find(_label, tmp->child)) != NULL) return result;
		}
		tmp = tmp->next;
	}while(tmp != _top);
	
	return NULL;
}

int CMenu::GetIndex(CMenuNode* _node){
	int index = 0;

	while(_node != _node->parent->child){
		_node = _node->prev;
		++index;
	}

	return index;
}

bool CMenu::Move(CMenuNode* &_result){
	_result = NULL;
	
	if (CheckHitKeyDown(KEY_INPUT_OK)){
		if (Cursor->child==NULL){
			_result = Cursor;
			return true;
		}else{
			Cursor = Cursor->child;
		}
	
	}else if (CheckHitKeyDown(KEY_INPUT_CANCEL)){
		if (Cursor->parent != NULL && Cursor->parent != god){
			Cursor = Cursor->parent;
		}

	/*
	}else if (CheckHitKeyDown(KEY_INPUT_RIGHT)){
		if (Cursor->next != NULL){
			Cursor = Cursor->next;
		}
	
	}else if (CheckHitKeyDown(KEY_INPUT_LEFT)){
		if (Cursor->prev != NULL){
			Cursor = Cursor->prev;
		}
	
	*/

	}else if (CheckHitKeyDown(KEY_INPUT_DOWN)){
		if (Cursor->next != NULL){
			Cursor = Cursor->next;
		}
	
	}else if (CheckHitKeyDown(KEY_INPUT_UP)){
		if (Cursor->prev != NULL){
			Cursor = Cursor->prev;
		}
	}

	return false;
}


void CBattleMenu::Draw(){
	if (!Alive) return;

	DrawBox(X, Y, X+Width, Y+Height, GetColor(30, 20, 80), true);
	DrawBox(X+5, Y+5, X+Width-5, Y+Height-5, GRAY, false);
	
	//母メニューの文字表示とカーソル表示
	CMenuNode* tmp = GetFront();
	for (int i=0; tmp!=NULL; i++){
		DrawString(X+30, Y+10+i*(1+GetFontSize()), tmp->label, WHITE, BLACK);
		if (Cursor == tmp) DrawString(X+8, Y+10+i*(1+GetFontSize()), "|>", WHITE, BLACK);
		tmp = tmp->next;
		
		if (tmp==GetFront()) break;
	}

	//子メニューの表示とカーソル表示
	if (Cursor->parent->child != tmp){	//tmp=front
		tmp = Cursor->parent->child;
		DrawBox(X+Width+5, Y, X+Width*4+5, Y+Height, GetColor(30, 20, 80), true);
		DrawBox(X+Width+5+5, Y+5, X+Width*4+5-5, Y+Height-5, GRAY, false);
		
		for (int i=0; tmp!=NULL; i++){
			DrawString(X+Width+5+30, Y+10+i*(1+GetFontSize()), tmp->label, WHITE, BLACK);
			if (Cursor == tmp) DrawString(X+Width+5+8, Y+10+i*(1+GetFontSize()), "|>", WHITE, BLACK);
			tmp = tmp->next;

			if (tmp==tmp->parent->child) break;
		}		
	}
}