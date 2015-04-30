#include "../Define.h"
#include "CmdList.h"

////////////////////////////////////////////////////////////////////////////////////////
///コマンドリスト///////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////


//最後尾に命令を追加
void CCmdList::Add(const char* _format, ...){
	if( strlen(_format)>256 ){
		ERRORDX("CmdList::Add 字数オーバー[255]：%s", _format);
		return;
	}

	DEBUGDX("_format=%s", _format);

	va_list args;
	char newText[1024];
	va_start(args, _format);
	vsprintf_s(newText, _format, args);	
	//va_end(args);

	DEBUGDX("ok");
	DEBUGDX("newText=%s", newText);

	if( strlen(newText)>256 ){
		ERRORDX("CmdList::Add 字数オーバー[255]：%s", newText);
		return;
	}

	node_tag* p = new node_tag;
	mystrcpy(p->command, newText);
	p->next = NULL;

	if (tail!=NULL)	tail->next = p;
	tail = p;

	if (front==NULL) front = p;
	
	va_end(args);
}


//先頭行の命令を返す
void CCmdList::Get(char* _cmd){
	node_tag* p = front;

	mystrcpy(_cmd, p->command);
	front = p->next;	
	if (p==tail) tail = NULL;

	delete p;	//コマンドリストから削除
}
bool CCmdList::Empty(){
	return (front == NULL);
}


void CCmdList::Clear(){
	
	node_tag* p=front;
	node_tag* tmp;

	/* 連結リストの要素を削除 */
	while( p != NULL ){
		tmp = p->next;
		delete p;
		p = tmp; 
	}
	
	front = NULL;
	tail = NULL;
}