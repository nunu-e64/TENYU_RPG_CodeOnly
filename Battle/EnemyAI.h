////多重インクルード防止（インクルードガード）//
#ifndef ENEMYAI_H						////
#define ENEMYAI_H						////
////////////////////////////////////////////////

class CActor;
class CEnemy;

#include "EnemyPlanner.h"
#include "EnemyTargetter.h"

class CEnemyAI{		//EnemySpeciesが持つ、plannerやTargeterを束ねる
public: 
	CEnemyAI(){
		CONSTRUCTED;
		Actor = NULL;
		PLAYER_NUM = 0;
		ENEMY_NUM = 0;

		for (int i = 0; i<MAX_PLAYER_NUM; i++){
			Attention[i] = 0;
		}

		//For Test$
		    Attention[0] = 10;
			Attention[1] = 5;
			Attention[2] = 8;

		for (int i=0; i<MAX_PLAYER_NUM; i++){
			ExtRate[i] = max(0, (i==0? 1.0: ExtRate[i-1]-0.25));
		}
	}

	~CEnemyAI(){
		DESTRUCTED;	
	}

	void BattleReady(const CActor* const* _actorList, const int _playerNum, const int _enemyNum){
		Planner->BattleReady(_actorList, _playerNum, _enemyNum);
		Targetter->BattleReady(_actorList, _playerNum, _enemyNum);
		Actor = _actorList;
		PLAYER_NUM = _playerNum;
		ENEMY_NUM = _enemyNum;
	}
	CEnemyPlanner* SetPlanner(CEnemyPlanner* _planner){
		Planner = _planner;
		Planner->SetRandomPlanSet(&RandomPlanSet);
		return _planner;
	}
	CEnemyTargetter* SetTargetter(CEnemyTargetter* _targetter){
		Targetter = _targetter;
		Targetter->SetAttention(&Attention[0]);
		return _targetter;
	}
	int GetPlan(const CEnemy* _enemy);
	int GetTarget(const CEnemy* _enemy);

	bool AddRandomPlanSet(const unsigned int _index, std::vector<std::pair<int, int> > _planList, bool _clear=false);
	void AddAttention(int _playerNum, attention_tag _value){
		Attention[_playerNum] = between(0, MAX_ATTENTION, Attention[_playerNum]+_value); 
	}

	static void SetAttentionCursorImage(int _index, int _img);
	void Draw(const CEnemy* _enemy);
	

private:
	CEnemyPlanner* Planner;
	CEnemyTargetter* Targetter;

	std::map <int, std::vector<std::pair<int, int> > > RandomPlanSet;
		//RandomPlanSet[index] = (choice, probability)
		//行動選択肢とその発動比を並べたリスト。
		//行動計算の為にAI.plannerとAI.Targetにポインタを渡しておく

	int Attention[MAX_PLAYER_NUM];
		//Enemyの各プレイヤーに対する注目度を示す

	//全アクターへのアクセスを持たせておく（戦闘開始ごとに更新）（EnemyAIは橋渡しなので実際に持つ必要はない）
		const CActor* const* Actor;
		int PLAYER_NUM;
		int ENEMY_NUM;

	static int AttentionCursor[MAX_PLAYER_NUM];
	double ExtRate[MAX_PLAYER_NUM];

};

////多重インクルード防止（インクルードガード）//
#endif										////
////////////////////////////////////////////////