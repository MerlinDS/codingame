#pragma once
#include <vector>
#include <map>

using namespace std;

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
	vector<int> gateways;
	map<int, Node*> network;
	Node* getNode(int const);
	inline Node* getNeighbor(Node* const, int);
	void clearNodes();
	Link* findGatewayLink(int, int, int&);
	bool isGatewayNeighbor(int, int);
};
