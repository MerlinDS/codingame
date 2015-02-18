#include "WallBuilder.h"
#include <string>

//========================== WallBuilder ============================
WallBuilder::WallBuilder(int w, int h) : width(w), height(h), 
	walls(vector<Point*>())
{}
WallBuilder::~WallBuilder()
{
	this->walls.clear();
}

void WallBuilder::addWall(int x, int y, string orientation)
{
	Point* s0 = nullptr; Point* s1 = nullptr;//segments of wall
	this->getWallSegments(x, y, orientation, s0, s1);
	for (auto i = 0; i < 2; i++)
		this->addSegment(i == 0 ? s0 : s1);

}

void WallBuilder::removeWall(int x, int y, string orientation)
{
	Point* s0 = nullptr; Point* s1 = nullptr;//segments of wall
	this->getWallSegments(x, y, orientation, s0, s1);
	for (auto i = 0; i < 2; i++)
		this->removeSegment(i == 0 ? s0 : s1);
}

const vector<Point*>* WallBuilder::getWalls()
{
	return &this->walls;
}

//private
int WallBuilder::getSegmentIndex(const Point* segment) const
{
	int i;
	for (i = 0; i < this->walls.size(); ++i)
		if (*this->walls[i] == *segment)break;//has wall
	return i == this->walls.size() ? -1 : i;
}

void WallBuilder::addSegment(Point* segment)
{
	if (this->getSegmentIndex(segment) < 0)
		this->walls.push_back(segment);
}

void WallBuilder::removeSegment(Point* segment)
{
	auto index = this->getSegmentIndex(segment);
	if (index > -1)this->walls.erase(this->walls.begin() + index);
}

void WallBuilder::getWallSegments(int x, int y, string orientation, Point*& s0, Point*& s1)
{
	//wall has two segments
	for (auto i = 0; i < 2; ++i){
		float x0, x1, y0, y1;
		x0 = x1 = static_cast<float>(x);
		y0 = y1 = static_cast<float>(y);
		if (orientation == "H"){
			x0 = static_cast<float>(x + i); x1 = x0 + 1;
		}
		else {
			y0 = static_cast<float>(y + i); y1 = y0 + 1;
		}

		if (i == 0)s0 = new Point(x0, x1, y0, y1);
		else s1 = new Point(x0, x1, y0, y1);
	}
}