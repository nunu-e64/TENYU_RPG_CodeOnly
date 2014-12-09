#include "Define.h"
#include "Menu.h"

void CMenu::Init(int _x, int _y, int _width, int _height){
	X = _x;
	Y = _y;
	Width = _width;
	Height = _height;
}

void CMenu::Create(const char _frontlabel[32]){

	Clear(front);
	front = new CMenuNode(_frontlabel);
	Cursor = front;
	Alive = false;

}

void CMenu::Add(const char _parentlabel[32], const char _newlabel[32]){
	CMenuNode* tmp = front;

	if (Find(_newlabel)!=NULL){
		ErrorDx("Error->CMenu::Add->Already existed name:%s", _newlabel);
		return;
	}

	if (!(strlen(_parentlabel))){
		while (tmp->next != NULL){ tmp = tmp->next;}
		tmp->next = new CMenuNode(_newlabel);
		tmp->next->prev = tmp;	

	}else{
		tmp = Find(_parentlabel);
		if (tmp == NULL){
			ErrorDx("Error->CMenu::Add->NotFound:%s",_parentlabel); 
		}else if (tmp->child == NULL){
			tmp->child = new CMenuNode(_newlabel);
			tmp->child->parent = tmp;
		}else{
			tmp = tmp->child;
			while (tmp->next != NULL){ tmp = tmp->next;}
			tmp->next = new CMenuNode(_newlabel);
			tmp->next->prev = tmp;	
			tmp->next->parent = tmp->parent;	
		}
	}
}

void CMenu::Clear(CMenuNode* _top){
	CMenuNode* tmp = _top;

	if (tmp==NULL) return;
	if (tmp->prev!=NULL) {
		ErrorDx("Error->CMenu::Clear", __FILE__, __LINE__);
		return;
	}

	if (tmp->parent!=NULL) tmp->parent->child=NULL;

	while (tmp != NULL){
		if (tmp->child!=NULL) Clear(tmp->child);
		CMenuNode* deletenode = tmp;
		tmp = tmp->next;
		if (tmp!=NULL) tmp->prev = NULL;
		delete deletenode;
	}
}

CMenuNode* CMenu::Find(const char _label[32], CMenuNode* _top){
	CMenuNode* tmp;
	if (_top == NULL) tmp = front; else tmp = _top; 

	while(tmp!=NULL){
		if (mystrcmp(tmp->label, _label)) return tmp;
		tmp = tmp->next;
	}

	CMenuNode* result;
	if (_top == NULL) tmp = front; else tmp = _top; 	while(tmp!=NULL){	
		if (tmp->child!=NULL){
			if ((result=Find(_label, tmp->child))!=NULL) return result;
		}		
		tmp = tmp->next;
	}
	
	return NULL;
}

int CMenu::GetIndex(CMenuNode* _node){
	int index=0;

	while(_node->prev!=NULL){
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
		if (Cursor->parent != NULL){
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
	CMenuNode* tmp = front;
	for (int i=0; tmp!=NULL; i++){
		DrawString(X+30, Y+10+i*(1+GetFontSize()), tmp->label, WHITE, BLACK);
		if (Cursor == tmp) DrawString(X+8, Y+10+i*(1+GetFontSize()), "|>", WHITE, BLACK);
		tmp = tmp->next;
	}

	//子メニューの表示とカーソル表示
	if (Cursor->parent!=NULL){
		tmp = Cursor->parent->child;
		DrawBox(X+Width+5, Y, X+Width*4+5, Y+Height, GetColor(30, 20, 80), true);
		DrawBox(X+Width+5+5, Y+5, X+Width*4+5-5, Y+Height-5, GRAY, false);
		for (int i=0; tmp!=NULL; i++){
			DrawString(X+Width+5+30, Y+10+i*(1+GetFontSize()), tmp->label, WHITE, BLACK);
			if (Cursor == tmp) DrawString(X+Width+5+8, Y+10+i*(1+GetFontSize()), "|>", WHITE, BLACK);
			tmp = tmp->next;
		}		
	}
}