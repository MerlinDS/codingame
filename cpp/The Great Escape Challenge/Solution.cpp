#include "Solution.h"
#include "WallInstaller.h"
#include "Player.h"
#include <iostream>

//========================== Solution ===============================
Solution::Solution(int w, int h, int id, int pc):
	width(w), height(h), myId(id), playerCount(pc),
	wallBuilder( new WallInstaller(w, h) )
{}
Solution::~Solution()
{
	delete this->wallBuilder;
}

void Solution::updatePlayer(int id, int x, int y, int wallsLeft)
{
	if (this->players.size() == id){
		this->players.push_back(new Player(id, this->playerCount - 1,
			this->width, this->height, 
			this->wallBuilder->getWalls()));
		cerr << "Player with id = " << id << " was created" << endl;
	}
	auto* player = this->players.at(id);
	player->update(x, y, wallsLeft);
}

void Solution::updateWall(int x, int y, string orientation)
{
	this->wallBuilder->addWall(x, y, orientation);
	cerr << orientation << "Wall( x = " << x << ", y = " << y << ") was updated" << endl;
}

string Solution::playerAction()
{
	this->calculatePath();
	this->getEmeny();

	auto* player = this->players.at(this->myId);	
	string action = "NONE";
	if (player->getId() != this->closeToFinish 
		&& player->hasWallsToSet())
	{
		cerr << "Need to build a wall for enemy "
			<< this->closeToFinish << " !!!" << endl;
		//copy all players positions to prevent changing
		auto players = new vector<Point>();
		for (auto i = 0; i < this->players.size(); ++i)
			players->push_back( Point( *this->players[i]->getPosition() ) );
		//get path
		auto* installer = static_cast<WallInstaller*>(this->wallBuilder);
		action = installer->buildFor(this->myId, closeToFinish, &this->players);
	}
	if (action == "NONE")
	{
		if (player->getDiraction() == player->getStartEdge())
		{
			cerr << "Go to bad dir" << endl;
		}
		action = player->getNextStep();
	}
	return action;
}

void Solution::calculatePath()
{
	for (auto i = 0; i < this->players.size(); ++i)
	{
		auto player = this->players.at(i);
		if (!player->isPlaying())continue;
		player->calculatePath();
//		cerr << endl;
	}
}

void Solution::getEmeny()
{
	this->closeToFinish = this->myId;
	auto* player = this->players.at(this->myId);
	if (player->isDusturbed())
	{
		cerr << "I ( id = " << this->myId << " ) was disturb!" << endl;
		cerr << "I has pass length " << player->pathLength() << endl;
		auto mpl = player->pathLength();
		//get enemies
		for (auto i = 0; i < this->players.size(); ++i)
		{
			auto p = this->players.at(i);
			if (p == player || !p->isPlaying())continue;
			if (mpl > p->pathLength()){
				mpl = p->pathLength();
				this->closeToFinish = i;
				cerr << "My enemy: " << i << " has path length " << p->pathLength() << endl;
			}
		}
	}
}