#include "PathFinder.h"
#include <vector>
#include <queue>

//========================== PathFinder =============================
PathFinder::PathFinder(int w, int h, const vector<Point*>* wa)
	:width(w), height(h), walls(wa)
{
	//init empty grid
	this->grid = vector <std::vector<int> >();
	for (auto x = 0; x < this->width; ++x){
		this->grid.push_back(vector<int>(this->height));
	}
	//init edeges
	this->edges[TOP][0] = Point(0, 0);
	this->edges[TOP][1] = Point(this->width - 1, 0);
	this->edges[LEFT][0] = Point(0, 0);
	this->edges[LEFT][1] = Point(0, this->height - 1);
	this->edges[RIGHT][0] = Point(this->width - 1, 0);
	this->edges[RIGHT][1] = Point(this->width - 1, this->height - 1);
	this->edges[BOTTOM][0] = Point(0, this->height - 1);
	this->edges[BOTTOM][1] = Point(this->width - 1, this->height - 1);
}

PathFinder::~PathFinder()
{
	//do not delete walls
	this->walls = nullptr;
}
void PathFinder::calculatePath(Point& position)
{
	this->calculatePath(position.x, position.y);
}

void PathFinder::calculatePath(int x, int y)
{
	this->cleanGrid();
	this->executeGridWave(x, y);
}

/* set all cells of grid to -1*/
void PathFinder::cleanGrid()
{
	for (auto x = 0; x < this->width; ++x){
		for (auto y = 0; y < this->height; ++y){
			this->grid[x][y] = -1;
		}
	}
}

void PathFinder::executeGridWave(int x, int y)
{
	//cerr << "Execute Grid Wave for x = " << x << ", y = " << y << endl;
	//set 0 weight to start position
	const auto n = 4;//count of neighbors for one node
	this->grid[x][y] = 0;
	queue<Point> q; q.push(Point(x, y));//push first point;
	while (!q.empty())
	{
		auto p = q.front(); q.pop();
		auto w = this->grid[p.x][p.y];
		//get neighbors
		for (auto i = 0; i < n; ++i)
		{
			//get neighbor point
			auto np = this->getNeighbor(static_cast<DIR>(i), p);
			if (np.x == -1 && np.y == -1)continue;//neighbor is out of grid
			/* 
				If neighbor weight was not set yet and not blocked by wall
				set new new weight for it.
			*/
			if (this->grid[np.x][np.y] < 0 &&
				!this->blockedByWall(p, np))
			{
				this->grid[np.x][np.y] = w + 1;
				q.push(np);
			}
		}
	}
}

/* Get target point from edge. */
Point PathFinder::getTarget(DIR edge, Point anchor)
{

	auto v = this->edges[edge][1] - this->edges[edge][0];
	v.normalize();
	auto a = anchor * v + this->edges[edge][0];
	//get edges
	auto t0 = this->edges[edge][0] - a;
	auto t1 = this->edges[edge][1] - a;
	auto n = abs(t0.length()) > abs(t1.length()) ? 
		abs(t0.length()) : abs(t1.length());
	t0.normalize(); t1.normalize();
	int minWeihgt = INT_MAX;
	auto target = Point(-1, -1);
	for (auto i = 0; i < n; ++i)
	{
		auto* p = this->getEndgePoint(t1, a, i);
		this->setTmpTarget(p, target, minWeihgt);
		p = this->getEndgePoint(t0, a, i);
		this->setTmpTarget(p, target, minWeihgt);
	}
	
	return target;
}

vector<Point>* PathFinder::getPath(const Point& end)
{
	auto* path = new vector<Point>();
	//get path form end to 0
	auto p = Point(end);
	while (this->grid[p.x][p.y])
	{
		auto weight = this->grid[p.x][p.y];
		path->push_back(p);
		//find next node and direction
		for (auto i = 0; i < 4; i++)
		{
			auto np = this->getNeighbor(static_cast<DIR>(i), p);
			if (np.x == -1 && np.y == -1)continue;//neighbor is out of grid
			if (this->grid[np.x][np.y] < weight
				&& !this->blockedByWall(p, np)){
				p = np;//next node found;
				break;
			}
		}
	}

	if (path->size() > 0){
		reverse(path->begin(), path->end());
	}
	return path;
}

Point PathFinder::getNeighbor(const DIR dir, const Point& p) const
{
	auto np = Point();//neightbor point
	np.x = dir == LEFT ? p.x - 1 : dir == RIGHT ? p.x + 1 : p.x;
	np.y = dir == TOP ? p.y - 1 : dir == BOTTOM ? p.y + 1 : p.y;

	if (np.x < 0 || np.x >= this->width ||
		np.y < 0 || np.y >= this->height){
		np.x = -1; np.y = -1;//this neighbor is out of grid
	}
	return np;
}

bool PathFinder::blockedByWall(const Point& p0, const Point& p1) const
{
	auto p = Point(p0.x, p1.x + 1, p0.y, p1.y + 1);
	auto it = this->walls->begin();
	for (; it != this->walls->end(); ++it)
		if (**it == p)break;//has wall
	return it != this->walls->end();;
}

inline Point* PathFinder::getEndgePoint(const Point& t, const Point& a, int i) const
{
	int x = t.x * i + a.x;
	int y = t.y * i + a.y;
	return x >= 0 && y >= 0 && x < this->width
		&& y < this->height ? new Point(x, y) : nullptr;
}

void PathFinder::setTmpTarget(Point* p, Point& t, int& minWeihgt)
{
	if (p == nullptr)return;
	auto weight = this->grid[p->x][p->y];
	if (weight < 0 || weight > minWeihgt)return;
	minWeihgt = weight;
	t = *p;
}