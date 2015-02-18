#pragma once
#include "PathFinder.h"

//========================== Player =================================
class Player :
	private PathFinder
{
public:
	Player(int, int, int, const vector<Point*>*);
	~Player();
	void update(int, int, int);
	void calculatePath();
	//getters
	bool isPlaying() const;
	bool hasWallsToSet() const;
	string getNextStep();
	int pathLength() const;
	int getId() const;
	const Point* getPosition() const;
	const vector<Point>* getPath() const;
	DIR getStartEdge() const;
	DIR getFinishEdge() const;
	bool pathHasAngle() const;
private:
	int id;
	int wallsLeft;
	DIR startEdge;
	DIR finishEdge;
	Point position;
	vector<Point>* path;
};

