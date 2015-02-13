#include "GatewayDefender.h"
#include <iostream>
#include <queue>
#include <string>


GatewayDefender::GatewayDefender(){}
GatewayDefender::~GatewayDefender(){}

void GatewayDefender::addLink(int from, int to)
{
	auto link = new Link{ from, to };
	this->getNode(from)->links.push_back(link);
	this->getNode(to)->links.push_back(link);
}

void GatewayDefender::setGatewayTo(int index)
{
	this->getNode(index)->status = STATUS::GATEWAY;
}

string GatewayDefender::setVirusTo(int index)
{
	this->clearNodes();
	//find nearest gateway node
	string result;
	auto* node = this->getNode(index);
	node->width = 0;
	queue<Node*> search; search.push(node);
	while (!search.empty())
	{
		node = search.front();
		for (int i = 0; i < node->links.size(); ++i)
		{
			auto link = node->links[i];
			if (link->blocked)continue;
			auto neighbor = this->getNeighbor(node, i);
			if (neighbor->status == GATEWAY)
			{
				result = to_string(link->node0) + " " + to_string(link->node1);
				link->blocked = true;
				return result;
			}

			auto width = node->width + 1;
			if (neighbor->width < 0 || neighbor->width > width)
			{
				neighbor->width = width;
				search.push(neighbor);
			}
			
		}
		search.pop();
	}
	return result;
}

GatewayDefender::Node* GatewayDefender::getNode(int const index)
{
	if (this->network.find(index) == this->network.end())
		this->network[index] = new Node{};
	return this->network[index];
}

GatewayDefender::Node* GatewayDefender::getNeighbor(Node* const node, int linkIndex)
{
	auto* link = node->links.at(linkIndex);
	auto* neightbor = this->getNode(link->node0);
	return node != neightbor ? neightbor : this->getNode(link->node1);
}

/* set all nodes in network as not visited*/
void GatewayDefender::clearNodes()
{
	for (auto it = this->network.begin(); it != this->network.end(); ++it)
	{
		auto* node = &(*it->second);
		node->width = -1;
	}
}
