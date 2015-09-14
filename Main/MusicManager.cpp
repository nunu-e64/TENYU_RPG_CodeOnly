#include "MusicManager.h"

void CMusicManager::Init()
{
	StopAllMusic();

	auto it = MusicBank.begin();
	for (; it != MusicBank.end(); it++) {
		DeleteSoundMem((*it).second);
	}
	
	MusicBank.clear();
}

void CMusicManager::LoadMusic(std::string _key, const char * _address)
{

	if (GetMusicHandle(_key, false) == -1) {
		int handle = LoadSoundMem(_address);

		if (handle != -1) {
			MusicBank[_key] = handle;
			PauseFlag[handle] = false;
		} else {
			ERRORDX("NotFoundMusicFile:key=%s:%s", _key.c_str(), _address);
		}

	} else {
		ERRORDX("Already Music Key (not registerd):%s:%s", _key.c_str(), _address);
	}

}

void CMusicManager::PlayMusic(std::string _key) {

	int handle = GetMusicHandle(_key);
	if (handle != -1) {

		if (PauseFlag[handle]) {
			PlaySoundMem(handle, DX_PLAYTYPE_BACK, false);//ポーズ位置から再生
			PauseFlag[handle]  = false;
		} else {
			PlaySoundMem(handle, DX_PLAYTYPE_BACK, true);	//通常再生（頭出し）
		}
	}
}

void CMusicManager::StopMusic(std::string _key){

	int handle = GetMusicHandle(_key);
	if (handle != -1) {

		StopSoundMem(handle);

		//ポーズ中に音楽停止させたときにはポーズフラグをリセットする
		PauseFlag[handle] = false;

	}

}

void CMusicManager::StopAllMusic() {

	auto it = MusicBank.begin();
	for (; it != MusicBank.end(); it++) {
		if (CheckSound((*it).second)) {
			StopSoundMem((*it).second);
			PauseFlag[(*it).second] = false;
		}
	}

}

void CMusicManager::PauseMusic(std::string _key)
{
	int handle = GetMusicHandle(_key);
	if (handle != -1) {

		if (CheckSoundMem(handle) == 1) {
			StopSoundMem(handle);
			PauseFlag[handle] = true;
		}
		
	}
}

void CMusicManager::ChangeNextMusicVolume(std::string _key, int _volumePercent)
{
	int handle = GetMusicHandle(_key);
	if (handle != -1) {
		ChangeNextPlayVolumeSoundMem(_volumePercent * 255 / 100, handle);
	}

}

int CMusicManager::GetMusicHandle(std::string _key, bool _errorAlert)
{
	if (MusicBank.find(_key) != MusicBank.end()) {
		return MusicBank[_key];
	}else{
		if (_errorAlert) {
			ERRORDX("NotFoundMusicKey:key=%s", _key.c_str());
		}
		return -1;
	}
}
