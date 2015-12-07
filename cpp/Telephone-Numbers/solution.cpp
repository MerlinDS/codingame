#include <iostream>
#include <string>
#include <map>
#include <algorithm>

using namespace std;

struct Node{
    map<int, Node*>children;
};

Node* head;
int counter;
void addTelephone(string telephone);
/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
int main()
{
    int N;
    counter = 0;
    head = new Node{};
    cin >> N; cin.ignore();
    for (int i = 0; i < N; i++) {
        string telephone;
        cin >> telephone; cin.ignore();
        addTelephone(telephone);
    }

    // Write an action using cout. DON'T FORGET THE "<< endl"
    // To debug: cerr << "Debug messages..." << endl;

    cout << counter << endl; // The number of elements (referencing a number) stored in the structure.
}

void addTelephone(string telephone){
    auto node = head;
    for(int i = 0; i < telephone.size(); ++i){
        int number = telephone.at(i) + '0';
        auto it = node->children.find(number);
        if(it == node->children.end()){
            node->children[number] = new Node{};
            counter++;
        }
        node = node->children[number];
    }
}
