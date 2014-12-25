#include "../Define.h"
#include "TrickManager.h"

void CTrickManager::Add(trick_tag trick){
	char256 tmpkey;
	mystrcpy(tmpkey.text, trick.Name);
		
	if (TrickBankLock) {
		WARNINGDX("'%s':TrickBank is Locked!", trick.Name);
		return;
	}

	if (TrickBank.find(tmpkey)==TrickBank.end()){
		TrickBank.insert( std::map<char256, trick_tag>::value_type( tmpkey, trick) );
	}else{
		ErrorDx("Error->AddNewTrick->Already exist name:%s", tmpkey.text);
	}
}

void CTrickManager::Add(char _name[32], int _power, int _cost, trick_tag::targetType_tag _targetType, std::string _damageEffectName, int _sideeffectnum, ...){
	trick_tag tmp;
	mystrcpy(tmp.Name, _name);
	tmp.Power = _power;
	tmp.Cost = _cost;
	tmp.TargetType = _targetType;
	tmp.DamageEffectIndex = GetTrickDamageEffectIndex(_damageEffectName);

	sideeffect_tag tmpeffect;

	if (TrickBankLock) {
		WARNINGDX("'%s':TrickBank is Locked!", _name);
		return;
	}

	if (_sideeffectnum%3!=0){
		ErrorDx("Error->TrickAdd->Number of arg[SideEffect] has something wrong.(Don'tAdd) %s",__FILE__, __LINE__, _name);
		return;
	}else{
		va_list args;
		va_start( args, _sideeffectnum);	//targetが大きすぎたときの処置方法はないのか？
		for (int i=0; i<_sideeffectnum/3; i++){
			tmpeffect.TrickEffect = va_arg(args, int);
			tmpeffect.Power = va_arg(args, int);
			tmpeffect.Incidence = va_arg(args, int);
			tmp.SideEffect.push_back(tmpeffect);
		}
		va_end(args);
	
		Add(tmp);
	}
}

void CTrickManager::Clear(){
	
	TrickBank.clear();
	TrickBankLock = false;
			
	//イテレータ使ってすべてdeleteしなくちゃいけない　
	std::vector <const CTrickDamageEffect*>::iterator it = TrickDamageEffectBank.begin();
	while(it!=TrickDamageEffectBank.end()){
		delete (*it);	
		++it;	//この時イテラータの指す先は正常か？	$
	}
	TrickDamageEffectBank.clear();

}

trick_tag const* CTrickManager::GetTrick(const char _name[32]){
	char256 tmpkey;
	mystrcpy(tmpkey.text, _name);

	if (TrickBank.find(tmpkey)!=TrickBank.end()) {
		TrickBankLock = true;
		return &TrickBank[tmpkey];;
	} else {
		ErrorDx("TrickManager->GetTrick->NotFound:%s", _name);
		return NULL;
	}
}

void CTrickManager::CreateDamageEffect(std::string _typeName, std::string _effectName, std::vector<std::string>_argList){

	for (unsigned int i=0; i<TrickDamageEffectBank.size(); i++){
		if (TrickDamageEffectBank[i]->GetName()==_effectName){
			ErrorDx("Error->CreateDamageEffect->Already Existed Name""%s"":Type""""", _effectName.c_str(), _typeName.c_str());
			return;
		}
	}

	if (_typeName=="BOMB"){
		TrickDamageEffectBank.push_back(new CTrickDamageEffect_BOMB(_effectName, _argList));
		
	}else if(_typeName=="PROTO"){
		TrickDamageEffectBank.push_back(new CTrickDamageEffect_PROTO(_effectName, _argList));

	//}else if(_typeName=="SPREAD"){
	
	}else{
		ErrorDx("Error->TrickEffectTypeName does't match any Effects.\n name:%s", _typeName.c_str());
		
	}
	
}

int CTrickManager::GetTrickDamageEffectIndex(std::string _name){
	if (sys::CheckStrNULL(_name.c_str())) {
		return -1;

	} else {
		for (unsigned int i=0; i<TrickDamageEffectBank.size(); i++){
			if (TrickDamageEffectBank[i]->GetName()==_name){
				return i;
			}
		}
		WarningDx("Warning->CTrickManager::GetTrickDamageEffectIndex\n->Not Found Effect:%s", _name.c_str());
		return -1;
	}	
}
