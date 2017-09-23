#include "CPlayer.h"
#include <d3d9.h>

int CPlayer::turn = 0;

CPlayer::CPlayer() {

	ball_name  = "NULL";
	score	   = GOAL_SCORE;
	hit_red1   = false;
	hit_red2   = false;
	hit_opponent  = false;
	
}

CPlayer::CPlayer(CSphere& ball) {

	ball_name  = ball.getName();
	score	   = GOAL_SCORE;
	hit_red1   = false;
	hit_red2   = false;
	hit_opponent  = false;

}

CPlayer::CPlayer(CSphere& ball, int goalScore) {

	ball_name  = ball.getName();
	score	   = goalScore;
	hit_red1   = false;
	hit_red2   = false;
	hit_opponent   = false;

	}

CPlayer::~CPlayer() {}


void CPlayer::setHitBall(CSphere& ball) {
	if (0 == ball.getName().compare("Red1"))
		this->hit_red1 = true;
	else if (0 == ball.getName().compare("Red2"))
		this->hit_red2 = true;
	else this->hit_opponent = true;
}

void CPlayer::decideScore() {
	if (hit_opponent) //°øÅ¸ 
		score += 10; 
	else if ( hit_red1 && hit_red2) {
		score -= 10;
	}
}


void CPlayer::create(CSphere& ball) {
	this->ball_name = ball.getName();
}

bool CPlayer::isMyBall(CSphere& ball){
	return 0 == (ball_name.compare(ball.getName()));
}