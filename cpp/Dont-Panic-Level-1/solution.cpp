#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

enum act{WAIT, BLOCK};
enum dir{NONE, LEFT, RIGHT};

struct Turn
{
    int cloneFloor; // floor of the leading clone
    int clonePos; // position of the leading clone on its floor
    dir direction; // direction of the leading clone: LEFT or RIGHT
    act action;
};
struct Floor{
    int index;
    int startPos;
    int exitPos;
    int blockPosition;
};
struct Level
{
    bool init;
    int nbFloors; // number of floors
    int width; // width of the area
    int nbRounds; // maximum number of rounds
    int nbTotalClones; // number of generated clones
    int nbAdditionalElevators; // ignore (always zero)
    vector<Floor> floors;
};
//
void findBlockPoints(Level&);
string turnAction(Level&, Turn&);
/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
int main()
{
    Level level;
    int nbElevators; // number of elevators
    int exitFloor; // floor on which the exit is found
    int exitPos; // position of the exit on its floor
    cin >> level.nbFloors >> level.width >> level.nbRounds >> exitFloor;
    cin >> exitPos >> level.nbTotalClones >> level.nbAdditionalElevators >> nbElevators; 
    cin.ignore();
    level.floors = vector<Floor>(level.nbFloors);
    for (int i = 0; i < nbElevators; i++) {
        int elevatorFloor; // floor on which this elevator is found
        int elevatorPos; // position of the elevator on its floor
        cin >> elevatorFloor >> elevatorPos; cin.ignore();
        level.floors[elevatorFloor].exitPos = elevatorPos;
    }
    level.floors[exitFloor].exitPos = exitPos;//for progremm exit is the same as elevator
    
    Turn turn;
    // game loop
    for(;;) {
        string direction; // direction of the leading clone: LEFT or RIGHT
        cin >> turn.cloneFloor >> turn.clonePos >> direction; cin.ignore();
        if(direction != "NONE")
            turn.direction = direction == "LEFT" ? dir::LEFT : dir::RIGHT;
        
        //first loop is for detecting position of start
        if(!level.init)
        {
            level.floors[0].startPos = turn.clonePos;
            findBlockPoints(level);
            level.init = true;
        }
        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;
        // cerr << turn.cloneFloor << " " << turn.clonePos << " " << turn.direction << endl; 
        cout << turnAction(level, turn) << endl; // action: WAIT or BLOCK
    }
}

string turnAction(Level& level, Turn& turn)
{
    Floor floor = level.floors[turn.cloneFloor];
    bool block = floor.blockPosition == turn.clonePos;
    if(!block){
        dir v = floor.exitPos - floor.startPos > 0 ? dir::RIGHT : dir::LEFT;
        block = v != turn.direction;
    }
    return block ? "BLOCK" : "WAIT";
}

void findBlockPoints(Level& level)
{
    cerr << "findBlockPoints" << endl;
    int n = level.floors.size();
    for(int i = 0; i < n; ++i)
    {
        Floor* floor = &level.floors[i];
        floor->index = i;//add floor index
        if(i > 0)//find all start positions
            floor->startPos = level.floors[i - 1].exitPos;
        
        int v = floor->exitPos - floor->startPos > 0 ? 1 : -1;
        floor->blockPosition = floor->exitPos + v;
        cerr << "block position = " << floor->blockPosition << endl;
    }
}
