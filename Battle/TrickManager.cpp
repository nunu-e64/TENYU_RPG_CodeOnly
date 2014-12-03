#include "../Define.h"
#include "TrickManager.h"

void CTrickManager::Add(trick_tag trick){
	char256 tmpkey;
	mystrcpy(tmpkey.text, trick.Name);
	
	//登録時に技名の重複チェックをしたい。map.findを使えばできるのか？$
	TrickBank.insert( std::map<char256, trick_tag>::value_type( tmpkey, trick) );
}

void CTrickManager::Add(char _name[32], int _power, int _cost, trick_tag::targetType_tag _targetType, int _sideeffectnum, ...){
	trick_tag tmp;
	mystrcpy(tmp.Name, _name);
	tmp.Power = _power;
	tmp.Cost = _cost;
	tmp.TargetType = _targetType;

	sideeffect_tag tmpeffect;

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
			
	//イテレータ使ってすべてdeleteしなくちゃいけない　
	std::vector <const CTrickDamageEffect*>::iterator it = TrickDamageEffectBank.begin();
	while(it!=TrickDamageEffectBank.end()){
		delete (*it);
		++it;
	}
	TrickDamageEffectBank.clear();

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
	//mapでは存在しないキーでアクセスしようとした場合、値をデフォルトコンストラクタで初期化し登録してくれやがる
}

void CTrickManager::CreateDamageEffect(std::string _typeName, std::string _effectName, ...){
	va_list args;
	va_start(args, _effectName);

	if (_typeName=="BOMB"){
		TrickDamageEffectBank.push_back(new CTrickDamageEffect_BOMB(_effectName, args));
		
	//}else if(_typeName=="SPREAD"){

	}else if(_typeName=="PROTO"){
		TrickDamageEffectBank.push_back(new CTrickDamageEffect_PROTO(_effectName, args));
	
	
	}else{
		ErrorDx("Error->TrickEffectTypeName does't match any Effects. name:%s", _typeName);
		
	}
	
	va_end(args);
}

int CTrickManager::GetTrickDamageEffectIndex(std::string _name){
	for (unsigned int i=0; i<TrickDamageEffectBank.size(); i++){
		if (TrickDamageEffectBank[i]->GetName()==_name){
			return i;
		}
	}
	return -1;
}

