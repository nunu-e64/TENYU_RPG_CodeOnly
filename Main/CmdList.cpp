#include "../Define.h"
#include "CmdList.h"

////////////////////////////////////////////////////////////////////////////////////////
///�R�}���h���X�g///////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////


//�Ō���ɖ��߂�ǉ�
void CCmdList::Add(const char* _format, ...){
	if( strlen(_format)>256 ){
		ERRORDX("CmdList::Add �����I�[�o�[[255]�F%s", _format);
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
		ERRORDX("CmdList::Add �����I�[�o�[[255]�F%s", newText);
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


//�擪�s�̖��߂�Ԃ�
void CCmdList::Get(char* _cmd){
	node_tag* p = front;

	mystrcpy(_cmd, p->command);
	front = p->next;	
	if (p==tail) tail = NULL;

	delete p;	//�R�}���h���X�g����폜
}
bool CCmdList::Empty(){
	return (front == NULL);
}


void CCmdList::Clear(){
	
	node_tag* p=front;
	node_tag* tmp;

	/* �A�����X�g�̗v�f���폜 */
	while( p != NULL ){
		tmp = p->next;
		delete p;
		p = tmp; 
	}
	
	front = NULL;
	tail = NULL;
}