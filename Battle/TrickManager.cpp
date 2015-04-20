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

void CTrickManager::Add(char _name[32], int _power, int _cost, trick_tag::targetType_tag _targetType, std::string _damageEffectName, std::vector<sideEffect_tag> sideEffectList){
	if (TrickBankLock) {
		WARNINGDX("'%s':TrickBank is Locked!(We don't add this trick.)", _name);
		return;
	}

	trick_tag tmpTrick;
	mystrcpy(tmpTrick.Name, _name);
	tmpTrick.Power = _power;
	tmpTrick.Cost = _cost;
	tmpTrick.TargetType = _targetType;
	tmpTrick.DamageEffectIndex = GetTrickDamageEffectIndex(_damageEffectName);
	tmpTrick.SideEffect = sideEffectList;	

	Add(tmpTrick);
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
