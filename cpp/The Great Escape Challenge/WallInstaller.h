#pragma once
#include "WallBuilder.h"
#include "PathFinder.h"
#include "Player.h"

//========================= WallInstaller ===========================
class WallInstaller :
	public WallBuilder, private PathFinder
{
public:
	WallInstaller(int, int);
	~WallInstaller();
	string buildFor(int, int, const vector<Player*>*);
	string buildFor(int, const vector<Player*>*);

private:
	struct Wall
	{
		Point s0;
		Point s1;
	};
	//data
	const Player* player;
	const vector<Point>* path;
	const vector<Player*>* players;
	//
	int myId;
	int pathIndex;
	Point position;//Current position for wall creating
	void clearData();//For current user//executte after buildFor result!!!!
	Wall* createWall();
	Point finishWall(Point&, Point&);
	Point finishWall(Point&, Point&, Point&);
	Point finishWall(Point&, Point&, Point&, Point&);
	bool isWallValid(Wall*);
	bool isWallValid(Point&, Point&);
	bool isSegmentValid(Point&);
	//
	inline Point getSegmentBetween(Point&, Point&) const;
	inline bool isOdd(Point&, Point&) const;
	inline DIR getDirection(const Point&, const Point&, bool = false) const;
	inline string wall2String(Wall*) const;
};