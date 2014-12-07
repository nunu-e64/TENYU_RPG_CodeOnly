#include "../Define.h"
#include "SpeciesManager.h"
#include "Player.h"


bool CPlayerSpeciesManager::CreateSpecies(const char* _name, int _maxhp, int _atk, int _def, int _spd, int _img){
	CPlayerSpecies newPlayer;
	newPlayer.SetValue(_name, _maxhp, _atk, _def, _spd);
	newPlayer.Img = _img;
	PlayerBank.insert( std::map<std::string, CPlayerSpecies>::value_type( _name, newPlayer) );

	return true;
}

bool CPlayerSpeciesManager::SetTrickList(const char* _name, std::vector <trick_tag const*> _trickList){
	if (PlayerBank.find(_name)!=PlayerBank.end()){
		PlayerBank[_name].TrickList = _trickList;
		return true;
	}else{
		ErrorDx("Error->Not Found Player. Name:%s", _name);
		return false;
	}
}

bool CPlayerSpeciesManager::SetMemberList(int _index, const char* _name){
	if (_index<0){
		ErrorDx("Error->SetMemberList->size error:%d", _index);
		return false;
	}else if (_index>=(int)(MemberList.size())){
		MemberList.push_back(GetSpecies(_name));
	}else{
		MemberList[_index] = GetSpecies(_name);
	}
	return true;
}
bool CPlayerSpeciesManager::SetMemberList(){	//パーティメンバーリストの初期化。戦闘に出ないであろうメンバーにも番号は振っておく
	MemberList.clear();
	std::map<std::string, CPlayerSpecies>::iterator it=PlayerBank.begin();
	while( it != PlayerBank.end()){
		MemberList.push_back(&(*it).second);
		++it;
	}
	return true;
}

CPlayerSpecies* CPlayerSpeciesManager::GetSpecies(const char* _name){
	if (PlayerBank.find(_name)!=PlayerBank.end()){
		return &PlayerBank[_name];
	}else{
		ErrorDx("Error->PlayerSpeciesManager->GetPlayerSpecies->NotFound:%s", __FILE__, __LINE__, _name);
		return &Dammy_Player;
	}
}
CPlayerSpecies* CPlayerSpeciesManager::GetSpecies(int _index){
	if (_index<0 || _index>=(int)(MemberList.size())){
		ErrorDx("Error->MemberList size error:%d", _index);
		return &Dammy_Player;
	}else{
		return MemberList[_index];
	}
}


void CPlayerSpeciesManager::CopyValue(int PLAYER_NUM, CPlayer* _player){

	//戦闘終了後、PlayerBankに保存すべき情報をコピーする

	for (int i=0; i<PLAYER_NUM; i++){
		PlayerBank[_player[i].GetName()].Hp = _player[i].Hp;
		//PlayerBank[_player[i].GetName()].Mp = _player[i].Mp;
		PlayerBank[_player[i].GetName()].Exp = _player[i].Exp;
	}

}


////////////////////////////////////////////////////////////////////////////////
//CEnemySpecies//
//////////////////////////////////////////////////////////////////////////////
void CEnemySpeciesManager::Clear(){
	myLog("%s::Clear()_start", typeid(*this).name());

	EnemyBank.clear();
	
	myLog("EnemyBank.clear()fin");
	
	std::map <int, std::map<int, encount_tag> >::iterator it = MapEncount.begin();
	while(MapEncount.size()>0 && it!=MapEncount.end()){
		myLog("MapEncount.second.clear");
		(*it).second.clear();
		++it;
	}
	MapEncount.clear();
	
	myLog("MapEncount.clear()fin");

	for(unsigned int i=0; i<EnemyPlannerBank.size(); i++){	
		myLog("delete EnemyPlannerBank[%d]", i);
		delete EnemyPlannerBank[i];	
	}
	myLog("%s::Clear()_fin", typeid(*this).name());

}

bool CEnemySpeciesManager::CreateSpecies(const char* _name, int _maxhp, int _atk, int _def, int _spd, int _img){
	CEnemySpecies newEnemy;
	newEnemy.SetValue(_name, _maxhp, _atk, _def, _spd);
	newEnemy.Img = _img;

	if (EnemyBank.find(_name)==EnemyBank.end()){
		EnemyBank.insert( std::map<std::string, CEnemySpecies>::value_type( _name, newEnemy) );
		return true;
	}else{
		ErrorDx("Error->Already existed EnemyName:%s", _name);
		return false;
	}
}

bool CEnemySpeciesManager::SetTrickList(const char* _name, std::vector <trick_tag const*> _trickList){
	if (EnemyBank.find(_name)!=EnemyBank.end()){
		EnemyBank[_name].TrickList = _trickList;
		return true;
	}else{
		ErrorDx("Error->%s->Not Found Enemy. Name:%s", __FUNCTION__, _name);
		return false;
	}
}

bool CEnemySpeciesManager::SetRandomPlanSet(const char* _name, unsigned int _index, std::vector<std::pair<int, int> > _planList){
	bool for_return = true;

	if (EnemyBank.find(_name)!=EnemyBank.end()){
		if (EnemyBank[_name].RandomPlanSet.find(_index)==EnemyBank[_name].RandomPlanSet.end()){
			
			EnemyBank[_name].RandomPlanSet[_index] = _planList;

		}else{
			ErrorDx("Error->%s.SetRandomPlanSet->Already Existed Key(don't override) :%d", _name, (int)_index);
			for_return = false;
		}
	}else{
		ErrorDx("Error->%s->Not Found Enemy. Name:%s", __FUNCTION__, _name);
		for_return = false;
	}

	return for_return;
}

bool CEnemySpeciesManager::SetEnemyPlanner(std::string _enemyName, std::string _typeName, std::vector<std::string> _argList){	
	bool for_return = true;
	
	if (EnemyBank.find(_enemyName)!=EnemyBank.end()){
		CEnemyPlanner* newAI;
		
		if (_typeName=="MYHP"){
			newAI = new CEnemyPlanner_MYHP(_enemyName, _argList, &EnemyBank[_enemyName].RandomPlanSet);

		}else if(_typeName=="PLAYERNUM"){
			newAI = new CEnemyPlanner_PLAYERNUM(_enemyName, _argList, &EnemyBank[_enemyName].RandomPlanSet);
	
		}else{
			ErrorDx("Error->PlannerTypeName does't match any PlanTypes :%s:%s", _typeName.c_str(), _enemyName.c_str());
			for_return = false;
		}
	
		if(for_return){
			EnemyBank[_enemyName].AI = newAI;
			EnemyPlannerBank.push_back(newAI);
			myLog("EnemyPlannerBank.push_back(newAI)");
		}

	}else{
		ErrorDx("Error->%s->Not Found Enemy. Name:%s", __FUNCTION__, _enemyName);
		for_return = false;
	}

	return for_return;
}



CEnemySpecies* CEnemySpeciesManager::GetSpecies(const char* _name){	
	if (EnemyBank.find(_name)!=EnemyBank.end()){
		return &EnemyBank[_name];
	}else{
		ErrorDx("EnemySpeciesManager->GetEnemySpecies->NotFound:%s", __FILE__, __LINE__, _name);
		return &Dammy_Enemy;
	}
}

bool CEnemySpeciesManager::SetMapEncount(int _mapnum, int _chipnum, int _encount){

	MAP_MAX_CHECK(_mapnum, false);

	if (_chipnum<0 || _chipnum>255){
		ErrorDx("Error->_chipnum should between(0,255) :%d", __FILE__, __LINE__, _chipnum);
		return false;
	}
	if (_encount<0 || _encount>1000){
		ErrorDx("Error->_encount should between(0,1000) :%d", __FILE__, __LINE__, _encount);
		return false;
	}

	MapEncount[_mapnum][_chipnum].encount = _encount;
	//DebugDx("MapEncount[_mapnum][_chipnum].encount:%d", MapEncount[_mapnum][_chipnum].encount);
	return true;

}
bool CEnemySpeciesManager::AddMapEncountParty(int _mapnum, int _chipnum, int _encount, std::vector<std::string> _party){

	MAP_MAX_CHECK(_mapnum, false);

	if (_chipnum<0 || _chipnum>255){
		ErrorDx("Error->_chipnum should between(0,255) :%d", __FILE__, __LINE__, _chipnum);
		return false;
	}

	encount_tag::party_tag tmp;
	
	for (unsigned int i=0; i<_party.size(); i++){
		tmp.party.push_back(GetSpecies(_party[i].c_str()));
		//DebugDx("tmp.GetName:%s", tmp.party[i]->GetName().c_str());
	}
	tmp.per = _encount;

	MapEncount[_mapnum][_chipnum].partyset.push_back(tmp) ;

	if (MapEncount[_mapnum][_chipnum].encount < 0 || MapEncount[_mapnum][_chipnum].encount >1000){
		MapEncount[_mapnum][_chipnum].encount = 0;
	}
	return true;
}

bool CEnemySpeciesManager::CheckEncount(int _mapnum, int _chipnum, std::vector<CEnemySpecies*> &_party_p){

	MAP_MAX_CHECK(_mapnum, false);
	if (_chipnum<0 || _chipnum>255){
		ErrorDx("Error->_chipnum should between(0,255) :%d", __FILE__, __LINE__, _chipnum);
		return false;
	}

		if (MapEncount.find(_mapnum)==MapEncount.end()) return false;
		if (MapEncount[_mapnum].find(_chipnum)==MapEncount[_mapnum].end()) return false;

	///////////////////////////////////////////////////////////////////////////////////////

	if (MapEncount[_mapnum][_chipnum].encount > rand()%1000){
		//エンカウント決定したので戦う敵パーティを決定

		int probability=0;
		int partynum = -1;
		for (unsigned int i = 0; i < MapEncount[_mapnum][_chipnum].partyset.size(); i++){
			probability += MapEncount[_mapnum][_chipnum].partyset[i].per;
		    if ((rand()%1000)/(double)1000 * probability < MapEncount[_mapnum][_chipnum].partyset[i].per){
				partynum = i;
			}
		}
		//DebugDx("MapEncount[_mapnum][_chipnum].encount:%d",MapEncount[_mapnum][_chipnum].encount);
		//DebugDx("EnemySpeciesManager:EncountPartySetSize:%d",MapEncount[_mapnum][_chipnum].partyset.size());
		//DebugDx("EnemySpeciesManager:EncountPartyNum:%d",partynum);

		if (partynum!=-1){
			_party_p = MapEncount[_mapnum][_chipnum].partyset[partynum].party;
			return true;
		}else{
			return false;
		}

	}else{
		return false;
	}
}