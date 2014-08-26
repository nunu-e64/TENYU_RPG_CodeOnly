#include "../Define.h"
#include "TrickManager.h"

void CTrickManager::Add(trick_tag trick){
	char256 tmpkey;
	mystrcpy(tmpkey.text, trick.Name);

	TrickBank.insert( std::map<char256, trick_tag>::value_type( tmpkey, trick) );
}

void CTrickManager::Add(char _name[32], int _power, int _cost, int _sideeffectnum, ...){
	trick_tag tmp;
	mystrcpy(tmp.Name, _name);
	tmp.Power = _power;
	tmp.Cost = _cost;

	sideeffect_tag tmpeffect;

	va_list args;
	va_start( args, _sideeffectnum);	//target‚ª‘å‚«‚·‚¬‚½‚Æ‚«‚Ìˆ’u•û–@‚Í‚È‚¢‚Ì‚©H
	for (int i=0; i<_sideeffectnum/3; i++){
		tmpeffect.TrickEffect = va_arg(args, int);
		tmpeffect.Power = va_arg(args, int);
		tmpeffect.Incidence = va_arg(args, int);
		tmp.SideEffect.push_back(tmpeffect);
	}
	va_end(args);
	
	Add(tmp);
}

trick_tag const* CTrickManager::GetTrick(const char _name[32]){
	trick_tag* tmp;
	char256 tmpkey;
	mystrcpy(tmpkey.text, _name);

	tmp = &TrickBank[tmpkey];

	if (mystrcmp(tmp->Name, _name)){
		return tmp;
	}else{
		ErrorDx("TrickManager->GetTrick->NotFound:%s", _name);
		return NULL;
	}
}