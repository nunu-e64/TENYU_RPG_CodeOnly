////多重インクルード防止（インクルードガード）//
#ifndef MUSICMANAGER_H						////
#define MUSICMANAGER_H						////
////////////////////////////////////////////////

class CMusicManager{
public:
	static CMusicManager* GetInstance() {
		static CMusicManager instance;
		return &instance;
	}
	~CMusicManager(){DESTRUCTED;}

	void Init();
	
	void LoadMusic(std::string _key, const char* _address);
	void PlayMusic(std::string _key);
	void StopMusic(std::string _key);
	void StopAllMusic();
	void PauseMusic(std::string _key);
	
	void ChangeNextMusicVolume(std::string _key, int _volumePercent);


private:

	int GetMusicHandle(std::string _key, bool _errorAlert = true);

	std::map <std::string, int> MusicBank;
	std::map <int, bool> PauseFlag;

	//単一性を保証(Singleton)////////////////////////////////
	CMusicManager() {
		CONSTRUCTED;
	}
	CMusicManager(const CMusicManager& hoge);
	CMusicManager& operator=(const CMusicManager& hoge);
	/////////////////////////////////////////////////////////
};

////多重インクルード防止（インクルードガード）//
#endif										////	
////////////////////////////////////////////////

