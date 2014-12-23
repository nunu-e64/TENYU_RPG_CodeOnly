////多重インクルード防止（インクルードガード）//
#ifndef ENEMYTARGETTER_H					////
#define ENEMYTARGETTER_H					////
////////////////////////////////////////////////

class CActor;
class CEnemy;

class CEnemyTargetter{ //EnemySpeciesのターゲット選択AI用のインターフェース
public: 
	CEnemyTargetter(std::string _enemyname){
		CONSTRUCTED;
		EnemyName = _enemyname;

		Actor = NULL;
		PLAYER_NUM = 0;
		ENEMY_NUM = 0;

		Attention[0] = 5;
		Attention[1] = 8;
		Attention[2] = 10;
	}

	~CEnemyTargetter(){
		DESTRUCTED;	
	}

	void BattleReady(const CActor* const* _actorList, const int _playerNum, const int _enemyNum){
		Actor = _actorList;
		PLAYER_NUM = _playerNum;
		ENEMY_NUM = _enemyNum;
	}

	std::string GetName() const{return EnemyName;}
	virtual int GetTarget(const CEnemy* _enemy)=0;

protected:
	int Attention[MAX_PLAYER_NUM];

	enum{
		ATTENTION_DAMAGE = 2,
		ATTENIOTN_DEFFENCE = -1
	};

	static const int ATTENTION_RATIO[MAX_PLAYER_NUM];

	//全アクターへのアクセスを持たせておく（戦闘開始ごとに更新）
		const CActor* const* Actor;
		int PLAYER_NUM;
		int ENEMY_NUM;

private:
	std::string EnemyName;	//主の名前。エラー出力用でしかない。だってアクセスはEnemySpeciesからなされるし呼び出し元情報が必要な時はthisを渡せば済む。
};



class CEnemyTargetter_DEFAULT: public CEnemyTargetter{
public:
	CEnemyTargetter_DEFAULT(std::string _name):CEnemyTargetter(_name){
		CONSTRUCTED;
	}
	~CEnemyTargetter_DEFAULT(){DESTRUCTED;}

	int GetTarget(const CEnemy* _enemy);	//共通
};



class CEnemyTargetter_REVERSE: public CEnemyTargetter{
public:
	CEnemyTargetter_REVERSE(std::string _name):CEnemyTargetter(_name){
		CONSTRUCTED;
	};
	~CEnemyTargetter_REVERSE(){DESTRUCTED;}

	int GetTarget(const CEnemy* _enemy); //共通
};


////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////