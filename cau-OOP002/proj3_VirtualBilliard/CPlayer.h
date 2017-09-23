#pragma once
#include "CObject.h"
#include "CSphere.h"
#include "d3dUtility.h"

#define	GOAL_SCORE 100

/*
player Ŭ������ ���� ������
�� ���� �ȸ���� ������ ������ �Ǵ���
�����Ǵ��� �÷��̾ �ϴ°��� �����ϴ°� �ƴϴ�
���� ���⼭ ������ �߻��Ѱ� virtual lego���� ���� ������ ������ red red yellow whit�� �ϴµ� �ű⼭ ������ ���� �κ��������� ����
red1 red �̷������� �������� Ȥ�� ��ģ�� ������ Ȯ�� �ٶ� 
�� ������ �����ϸ�
1.������� ��Ȳ�� �����ĳ��ϱ����Ͽ�
2.������ ���� ���� ������ red1 red ������ ������ ���� ������ �Ǵ��ϱ� ���Ͽ� 
3. ���ϰ� �÷��̳ʴ� �ٸ���
 4. �÷��̾�� ���ǼӼ��� �ο��ϰ� �� 
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