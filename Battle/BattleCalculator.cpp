#include "../Define.h"
#include "BattleCalculator.h"


int CBattleCalculator::CalcGold(int _lv, int _goldgene){
	return max(1,_lv*_goldgene);
}

int CBattleCalculator::CalcExp(int _lv, int _expgene){
	return max(1,_lv*_expgene);
}
