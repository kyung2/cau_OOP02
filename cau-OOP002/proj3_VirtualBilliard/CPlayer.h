#pragma once
#include "CObject.h"
#include "CSphere.h"
#include "d3dUtility.h"

#define	GOAL_SCORE 100

/*
player 클래스를 만든 이유는
이 것을 안만드면 공에서 점수를 판단함
점수판단은 플레이어가 하는거지 공이하는게 아니다
또한 여기서 문제가 발생한게 virtual lego에서 공에 색깔을 입힐떄 red red yellow whit로 하는데 거기서 문제가 생긴 부분잇을수도 잇음
red1 red 이런식으로 입혓으니 혹시 놓친게 있으면 확인 바람 
즉 간단히 정리하면
1.점수얻는 상황을 쉽게파난하기위하여
2.유저의 공은 수구 위에서 red1 red 떄문에 유저의 공을 수ㄱ로 판단하기 위하여 
3. 공하고 플레이너느 다른다
 4. 플레이어에게 공의속성을 부여하게 함 
*/

class CPlayer  {
public:

	static int turn;

	CPlayer();

	CPlayer(CSphere& ball);
	CPlayer(CSphere& ball, int goalScore);

	~CPlayer();

	void setHitBall(CSphere& ball);

	void decideScore();				//reduce score from goal score
	void create(CSphere& ball);
	bool isMyBall(CSphere& ball);
	inline int getScore() {		return score; 	}; 
	inline void resetHit() {
		hit_red1 = false;
		hit_red2 = false;
		hit_opponent = false;
	}
	inline bool isOneMoreTurn(){
		return !hit_opponent && hit_red1 && hit_red2;
	}
	inline bool endGame(){
		if (score <= 0)	return true;
		else		    return false;
	}
	inline static void switchTurn(){ turn = 1 - turn; }

private:

	std::string ball_name;

	int score;

	bool hit_red1;
	bool hit_red2;
	bool hit_opponent;

};