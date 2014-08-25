#include "../Define.h"
#include "CmdList.h"

////////////////////////////////////////////////////////////////////////////////////////
///�R�}���h���X�g///////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////


//�Ō���ɖ��߂�ǉ�
void CCmdList::Add(const char* _data){

	if( strlen(_data)>256 ){
		ErrorDx("Error->CmdList::Add �����I�[�o�[[255]�F%s", __FILE__, __LINE__, _data);
		return;
	}

	node_tag* p = new node_tag;
	mystrcpy(p->command, _data);
	p->next = NULL;

	if (tail!=NULL)	tail->next = p;
	tail = p;

	if (front==NULL) front = p;
}


//�擪�s�̖��߂�Ԃ�
void CCmdList::Get(char* _cmd){
	node_tag* p = front;

	mystrcpy(_cmd, p->command);
	front = p->next;	
	if (p==tail) tail = NULL;

	delete p;
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