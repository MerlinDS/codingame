#pragma once
#include <string>
#include <vector>
#include "WallBuilder.h"
#include "Player.h"

using namespace std;

//========================== Solution ===============================
class Solution
{
public:
	/* width, height, myId, playerCount*/
	explicit Solution(int = 9, int = 9, int = 0, int = 2);
	virtual ~Solution();
	void updatePlayer(int, int, int, int);
	void updateWall(int, int, string);
	string playerAction();
private:
	int width, height;
	int myId, playerCount;
	int closeToFinish;
	WallBuilder* wallBuilder;
	vector<Player*> players;
	//methods
	void calculatePath();
	void getEmeny();
	void getCloser();
};

