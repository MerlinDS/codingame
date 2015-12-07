#include "GatewayDefender.h"
#include <iostream>
#include <queue>
#include <string>
#include <algorithm>


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
	this->gateways.push_back(index);
}

string GatewayDefender::setVirusTo(int index)
{
	this->clearNodes();
	//find nearest gateway node
	struct Data{ Link* link; int lenght; int neighbors; bool nearOtherGateway; };
	queue<Data*> data;
	for (int i = 0; i < this->gateways.size(); ++i)
	{
		auto length = 0;
		auto gateway = this->gateways[i];
		auto* link = this->findGatewayLink(gateway, index, length);
		if (link != nullptr)
		{
			auto nearOtherGateway = this->isGatewayNeighbor(link->node1, gateway);
			data.push(new Data{ link, length, this->getNode(gateway)->links.size(), nearOtherGateway });
		}
	}
	//deside what link to choose
	Data* result = nullptr;
	while (!data.empty())
	{
		auto d = data.front(); data.pop();
		if (result == nullptr)
		{
			result = d;
			continue;
		}
		//
		if (d->lenght < result->lenght)result = d;
		else if (d->lenght == result->lenght)
		{
			if (d->neighbors > result->neighbors)
				result = d;
			else if (d->neighbors == result->neighbors)
			{
				if (d->nearOtherGateway)
					result = d;
			}
		}
	}

	result->link->blocked = true;
	auto output = to_string(result->link->node0) + " " + to_string(result->link->node1);
	return output;
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
		node->status = NONE;
	}
}

GatewayDefender::Link* GatewayDefender::findGatewayLink(int gatewayIndex, int virusIndex, int& length)
{
	this->clearNodes();
	this->getNode(gatewayIndex)->status = GATEWAY;
	//find nearest gateway node
	auto* node = this->getNode(virusIndex);
	node->width = 0;
	queue<Node*> search; search.push(node);
	Link* link = nullptr;
	while (!search.empty())
	{
		node = search.front();
		for (int i = 0; i < node->links.size(); ++i)
		{
			if (node->links[i]->blocked)continue;

			link = node->links[i];
			auto width = node->width + 1;
			auto neighbor = this->getNeighbor(node, i);
			if (neighbor->status == GATEWAY){
				length = width;
				break;
			}
			
			if (neighbor->width < 0 || neighbor->width > width)
			{
				neighbor->width = width;
				search.push(neighbor);
			}
			link = nullptr;

		}
		search.pop();
		if (link != nullptr)break;
	}
	return link;
}

bool GatewayDefender::isGatewayNeighbor(int nodeIndex, int ignore)
{
	auto* node = this->getNode(nodeIndex);
	for (int i = 0; i < node->links.size(); i++)
	{
		auto* link = node->links[i];
		if (node->links[i]->blocked)continue;
		auto index = link->node0 == nodeIndex ? link->node1 : link->node0;
		if (index == ignore)continue;
		if (find(this->gateways.begin(), this->gateways.end(), index) != this->gateways.end())
			return true;
	}
	return false;
}
