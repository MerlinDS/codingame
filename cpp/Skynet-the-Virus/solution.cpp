#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <queue>

using namespace std;

//class difinition
class GatewayDefender
{
public:
	GatewayDefender();
	~GatewayDefender();
	void addLink(int, int);
	void setGatewayTo(int);
	string setVirusTo(int);
private:
	enum STATUS{NONE, GATEWAY, VIRUS};
	struct Link
	{
		int node0;
		int node1;
		bool blocked;
	};
	struct Node
	{
		STATUS status = NONE;
		vector<Link*> links;
		int width;
	};
	map<int, Node*> network;
	Node* getNode(int const);
	inline Node* getNeighbor(Node* const, int);
	void clearNodes();
};
/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
int main()
{
    auto* defender = new GatewayDefender();
    int N; // the total number of nodes in the level, including the gateways
    int L; // the number of links
    int E; // the number of exit gateways
    cin >> N >> L >> E; cin.ignore();
    for (int i = 0; i < L; i++) {
        int N1; // N1 and N2 defines a link between these nodes
        int N2;
        cin >> N1 >> N2; cin.ignore();
        defender->addLink(N1, N2);
    }
    for (int i = 0; i < E; i++) {
        int EI; // the index of a gateway node
        cin >> EI; cin.ignore();
        defender->setGatewayTo(EI);
    }

    // game loop
    while (1) {
        int SI; // The index of the node on which the Skynet agent is positioned this turn
        cin >> SI; cin.ignore();

        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;
        cout << defender->setVirusTo(SI) << endl; // Example: 0 1 are the indices of the nodes you wish to sever the link between
    }
}


//class implementation

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
