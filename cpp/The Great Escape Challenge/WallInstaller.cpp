#include "WallInstaller.h"
#include <string>

//========================= WallInstaller ===========================
WallInstaller::WallInstaller(int w, int h) :
	WallBuilder(w, h),
	PathFinder(w, h, this->getWalls())
{}

WallInstaller::~WallInstaller(){}

string WallInstaller::buildFor(int playerId, const vector<Player*>* players)
{
	//init
	this->players = players;
	this->player = this->players->at(playerId);
	this->path = player->getPath();
	//start
	Wall* wall = nullptr;
	this->pathIndex = 0;
	position = *this->player->getPosition();	
	while (wall == nullptr)
	{
		if (this->path->size() <= this->pathIndex)break;
		wall = this->createWall();
		// wall cannot be created or do not need to be created
		if (wall == nullptr)break;		
		//validate wall for blockind and other issues
		if (!this->isWallValid(wall))
		{
			if (this->path->size() - 1 == this->pathIndex + 1)
				break;//path is finished;
			//go to next position in path
			this->position = this->path->at(this->pathIndex);
			this->pathIndex++;
			wall = nullptr;
		}
	}

	this->clearData();
	return this->wall2String(wall);
}

void WallInstaller::clearData()
{
	//clear data after calculation
	this->pathIndex = 0;
	this->players = nullptr;
	this->path = nullptr;
}

WallInstaller::Wall* WallInstaller::createWall()
{
	Wall* wall = nullptr;
	auto from = this->position;
	auto to = this->path->at(this->pathIndex);
	auto next = this->path->size() <= this->pathIndex ?
		this->path->at(this->pathIndex + 1) : Point();

	auto dir = this->getDirection(from, to);
	//do not build the wall if other case
	if (dir != player->getStartEdge() )
	{
		//get first segment
		auto s0 = this->getSegmentBetween(from, to);
		//No need for segment validation: path cannot lay through the wall
		auto s1 = dir == player->getFinishEdge() ?
			this->finishWall(s0, from, to) :
			this->finishWall(s0, from, to, next);
		//TODO find first segment
		wall = new Wall{ s0, s1 };
	}
	return wall;
}

Point WallInstaller::finishWall(Point& s, Point& ns)
{
	/*
	Segment validation: for case when first try of creation was failed.
	And validation of new wall. Wall shouldn't cross other walls
	*/
	if (this->isSegmentValid(ns) && this->isWallValid(s, ns))
		return ns;
	return Point(-1, -1);
}

/*create segment by parity */
Point WallInstaller::finishWall(Point& s, Point& from, Point& to)
{
	auto isOdd = this->isOdd(from, to);
	auto ns = Point(s);//neighbor for start segment of the wall
	auto dir = this->getDirection(from, to);//perpendicular to wall
	if (dir == TOP || dir == BOTTOM)//orientation = H
	{
		ns.x += isOdd ? -1 : 1;
		//return to previous value and try other side if segment is invalid
		if (!this->isSegmentValid(ns) || !this->isWallValid(s, ns))
		{
			ns = Point(s);
			ns.x += !isOdd ? -1 : 1;
		}
	}
	else //orientation = V
	{
		ns.y += isOdd ? -1 : 1;
		//return to previous value and try other side if segment is invalid
		if (!this->isSegmentValid(ns) || !this->isWallValid(s, ns))
		{
			ns = Point(s);
			ns.y += !isOdd ? -1 : 1;
		}
	}
	return this->finishWall(s, ns);
}

/*create segment by path direction*/
Point WallInstaller::finishWall(Point& s, Point& from, Point& to, Point& next)
{
	auto dir = this->getDirection(from, to);//perpendicular to wall
	auto side = this->getDirection(to, next, true);
	auto ns = Point(s);
	if (dir == TOP || dir == BOTTOM)//orientation = H
	{
		ns.x += side == LEFT ? -1 : 1;
		//return to previous value and try other side if segment is invalid
		if (!this->isSegmentValid(ns) || !this->isWallValid(s, ns))
		{
			ns = Point(s);
			ns.x += side != LEFT ? -1 : 1;
		}
	}
	else //orientation = V
	{
		ns.y += side == TOP ? -1 : 1;
		//return to previous value and try other side if segment is invalid
		if (!this->isSegmentValid(ns) || !this->isWallValid(s, ns))
		{
			ns = Point(s);
			ns.y += side != TOP ? -1 : 1;
		}
	}
	return this->finishWall(s, ns);
}

bool WallInstaller::isWallValid(Wall* wall)
{
	//previos methods can not create one of the wall segment
	if (wall->s0.x + wall->s0.y < 0
		|| wall->s1.x + wall->s1.y < 0)return false;
	//TODO validate for path blockind
	//add virtual wall to walls
	this->addSegment(&wall->s0);
	this->addSegment(&wall->s1);
	//get path for players
	auto blocked = false;
	for (auto i = 0; i < this->players->size(); ++i)
	{
		const Player* player = this->players->at(i);
		if (!player->isPlaying())continue;
		auto p = Point(*player->getPosition());
		this->calculatePath( p );
		p = this->getTarget(player->getFinishEdge(), p);
		if (p.x + p.y < 0)
		{
			blocked = true;
			break;
		}
	}
	//remove virtual wall
	this->removeSegment(&wall->s0);
	this->removeSegment(&wall->s1);
	return !blocked;
}

bool WallInstaller::isWallValid(Point& s0, Point& s1)
{
	//find cross walls
	auto v = s1 - s0;
	v.x /= 2; v.y /= 2;
	auto s = Point(s0) + v;
	v.reverse();
	s = s + v;
	if (this->getSegmentIndex(&s) >= 0)//crossing one wall segment
	{
		s = s - v;//return previous value
		s = s - v;//check for next segment crossing
		return this->getSegmentIndex(&s) < 0;
	}
	return true;
}

bool WallInstaller::isSegmentValid(Point& s)
{
	//lays in positive borders
	auto valid = s.x > 0 && s.x < WallBuilder::width &&
		s.y > 0 && s.y < WallBuilder::height;
	//segment was not added befor
	return !valid ? false : this->getSegmentIndex(&s) < 0;
}

Point WallInstaller::getSegmentBetween(Point& a, Point& b) const
{
	auto c = a + b;
	c.x++; c.y++;
	c.x /= 2; c.y /= 2;
	return c;
}

bool WallInstaller::isOdd(Point& from, Point& to) const
{
	auto v = to - from;
	v.reverse();
	v = v * to;
	return static_cast<int>(v.x) % 2 
		+ static_cast<int>(v.y) % 2 == 0;
}

PathFinder::DIR WallInstaller::getDirection(
	const Point& from, const Point& to, bool opposite) const
{
	auto v = Point(to) - Point(from);
	if (opposite)
	{
		v.x *= -1; v.y *= -1;
	}
	if (v.x < 0)return LEFT;
	if (v.x > 0)return RIGHT;
	if (v.y > 0)return BOTTOM;
	if (v.y < 0)return TOP;
	throw -1;
}

string WallInstaller::wall2String(Wall* wall) const
{
	if (wall == nullptr)return "NONE";
	auto dir = this->getDirection(wall->s0, wall->s1);
	string orientation = dir == LEFT || dir == RIGHT ? "H" : "V";
	//find first segment 
	auto v = wall->s1 - wall->s0;
	auto c = v.x + v.y > 0 ? wall->s0 : wall->s1;
	auto wx = static_cast<int>(floor(c.x));
	auto wy = static_cast<int>(floor(c.y));
	return to_string(wx) + " " + to_string(wy) + " " + orientation;
}