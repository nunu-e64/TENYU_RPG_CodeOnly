#include "../Define.h"
#include "PlayerSpeciesManager.h"
#include "Player.h"


void CPlayerSpeciesManager::Clear(){
	Gold = 0; 
	PlayerBank.clear(); 
	PlayerBankLock = false;
	MemberList.clear();

}

bool CPlayerSpeciesManager::CreateSpecies(const char* _name, int _maxhp, int _atk, int _def, int _spd, int _img){
	CPlayerSpecies newPlayer;
	newPlayer.SetValue(_name, _maxhp, _atk, _def, _spd);
	newPlayer.Img = _img;

	if (PlayerBankLock) {
		WARNINGDX("'%s':PlayerBank is Locked!", _name);
		return false;
	}

	if (PlayerBank.find(_name)==PlayerBank.end()){
		PlayerBank.insert( std::map<std::string, CPlayerSpecies>::value_type( _name, newPlayer) );
		return true;
	}else{
		ErrorDx("Error->Already existed PlayerName:%s", _name);
		return false;
	}
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
bool CPlayerSpeciesManager::SetMemberList(){	//�p�[�e�B�����o�[���X�g�̏������B�퓬�ɏo�Ȃ��ł��낤�����o�[�ɂ��ԍ��͐U���Ă���
	MemberList.clear();
	std::map<std::string, CPlayerSpecies>::iterator it=PlayerBank.begin();
	if (it!=PlayerBank.end()) PlayerBankLock = true;
	while( it != PlayerBank.end()){
		MemberList.push_back(&(*it).second);
		++it;
	}
	return true;
}

CPlayerSpecies* CPlayerSpeciesManager::GetSpecies(const char* _name){
	if (PlayerBank.find(_name)!=PlayerBank.end()){
		PlayerBankLock = true;	//�Ȍ�PlayerBank�̗v�f�A�h���X���ς��Ȃ��悤�ɗv�f�̒ǉ����ւ���
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
		PlayerBankLock = true;
		return MemberList[_index];
	}

}


void CPlayerSpeciesManager::CopyValue(int PLAYER_NUM, CPlayer* _player){

	//�퓬�I����APlayerBank�ɕۑ����ׂ������R�s�[����

	for (int i=0; i<PLAYER_NUM; i++){
		PlayerBank[_player[i].GetName()].Hp = _player[i].Hp;
		//PlayerBank[_player[i].GetName()].Mp = _player[i].Mp;
		PlayerBank[_player[i].GetName()].Exp = _player[i].Exp;
	}

}

bool CPlayerSpeciesManager::CheckAfterLoad(){
	bool forReturn = true;

	std::map <std::string, CPlayerSpecies> ::iterator it = PlayerBank.begin();
	while(it!=PlayerBank.end()){
		if ((*it).second.TrickList.empty()) {
			ERRORDX("%s: Set Trick List for All Player!",(*it).second.GetName().c_str());
			forReturn = false;
		}
		++it;
	}

	return forReturn;
}
