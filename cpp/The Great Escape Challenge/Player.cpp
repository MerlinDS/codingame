#include "Player.h"
#include <iostream>

//========================== Player =================================
Player::Player(int myId, int enemiesCount, int w, int h, const vector<Point*>* walls) :
	PathFinder(w, h, walls), id(myId), enemiesCount(enemiesCount)
{
	//get start Edge for player
	this->startEdge = id == 0 ? LEFT : id == 1 ? RIGHT : TOP;
	//finish edge is opposite for start edge
	this->finishEdge = this->startEdge == LEFT ? RIGHT :
		this->startEdge == RIGHT ? LEFT :
		this->startEdge == TOP ? BOTTOM :
		/*this->startEdge == BOTTOM ? */ TOP;
}

Player::~Player()
{
	this->path->clear();
	delete this->path;
}

void Player::update(int x, int y, int wallsLeft)
{
	this->position.x = x;
	this->position.y = y;
	this->wallsLeft = wallsLeft;
	//clear previous path
	if (this->path != nullptr)
	{
		this->path->clear();
		delete this->path;
		this->path = nullptr;
	}
}

void Player::calculatePath()
{
	PathFinder::calculatePath(this->position);
	//find path to finish edge
	auto start = PathFinder::getTarget(this->finishEdge, this->position);
	if (start.x < 0 && start.y < 0)
	{
		cerr << "Target for player " << this->id << " was not found" << endl;
	} else
	{
		auto* path = PathFinder::getPath(start);
		if (path->size() > 0)
		{
			cerr << "Path was calculated for player " << this->id << endl;
			this->path = path;
		} else
		{
			cerr << "Path for player " << this->id << " was not found" << endl;
		}
	}
}

bool Player::isPlaying() const
{
	return this->position.x != -1 && this->position.y != -1;
}

bool Player::hasWallsToSet() const
{
	return this->wallsLeft > 0;
}

string Player::getNextStep()
{
	auto p = this->path->at(0) - this->position;
	auto step = "ERROR";
	if (p.x > 0)step = "RIGHT";
	if (p.x < 0)step = "LEFT";	
	if (p.y > 0)step = "DOWN";
	if (p.y < 0)step = "UP";
	return step;
}

int Player::pathLength() const
{
	return this->path->size();
}

int Player::getId() const
{
	return this->id;
}

const Point* Player::getPosition() const
{
	return &this->position;
}

const vector<Point>* Player::getPath() const
{
	return this->path;
}

PathFinder::DIR Player::getStartEdge() const
{
	return this->startEdge;
}

PathFinder::DIR Player::getFinishEdge() const
{
	return this->finishEdge;
}

bool Player::pathHasAngle() const
{
	if (this->path->size() < 2)return false;
	auto p = Point(this->position) - this->path->at(0);
	for (auto i = 0; i < this->path->size() - 1; ++i)
	{
		auto v = this->path->at(i) - this->path->at(i + 1);
		if (v.x != p.x || v.y != p.y)return true;
	}
	return false;
}

bool Player::isDusturbed()
{
	if (!this->disturbed && this->path->size() >  0)
	{
		this->disturbed = this->pathHasAngle();
		//auto disturb on end of path
		if (!this->disturbed)
		{
			this->disturbed = this->path->size() < this->id + this->enemiesCount;
		}

	}
	return this->disturbed;
}