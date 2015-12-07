#pragma once
#include <vector>
#include "Point.h"

using namespace std;

//========================== WallBuilder ============================
class WallBuilder 
{
public:
	WallBuilder(int, int);
	virtual ~WallBuilder();
	void addWall(int, int, string);
	void removeWall(int, int, string);
	//gettersP
	const vector<Point*>* getWalls();
protected:
	int width;
	int height;
	int getSegmentIndex(const Point*) const;
	inline void addSegment(Point*);
	inline void removeSegment(Point*);
private:
	vector<Point*> walls;
	inline void getWallSegments(int, int, string, Point*&, Point*&);
};

