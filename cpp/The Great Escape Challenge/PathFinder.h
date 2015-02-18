#pragma once
#include <vector>
#include "Point.h"
using namespace std;

//========================== PathFinder =============================
class PathFinder
{
public:
	enum DIR{ TOP = 0, LEFT, RIGHT, BOTTOM };
	PathFinder(int, int, const vector<Point*>*);
	virtual ~PathFinder()=0;
	virtual void calculatePath(Point&);
	virtual void calculatePath(int, int);	
protected:
	//maybe a private one
	int width;
	int height;
	const vector<Point*>* walls;
	vector<vector<int>> grid;//grid for waves of path finding
	Point edges[BOTTOM + 1][2];//vectors of edges
	//methods
	void cleanGrid();
	void executeGridWave(int, int);
	Point getTarget(DIR, Point);
	vector<Point>* getPath(const Point&);
private:
	inline Point getNeighbor(const DIR, const Point&) const;
	inline bool blockedByWall(const Point&, const Point&) const;
	inline Point* getEndgePoint(const Point&, const Point&, int) const;
	inline void setTmpTarget(Point*, Point&, int&);
};

