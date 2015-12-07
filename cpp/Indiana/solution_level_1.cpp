#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
int leftBlock[] = {0, 3, 5, 7, 11};
int rightBlock[] = {0, 3, 4, 9, 10};
int downBlock[] = {0, 2, 6, 10, 11};
bool canMoveFuther(int type, int blocking[], int size){
    for(int i = 0; i < size; i++)
    {
        if(type == blocking[i])
        {
            cerr << "blocked by type" + to_string(type) << endl;
            return false;
        }
    }
        
    return true;
}

int getX(vector<vector<int>> & greed, int x, int y, string pos){
    bool canMoveLeft = canMoveFuther(greed[y][x], leftBlock, 5);
    bool canMoveRight = canMoveFuther(greed[y][x], rightBlock, 5);
    if(pos == "LEFT" && canMoveRight)
    {
        cerr << "move right" << endl;
        x++;
    }
    else if(pos == "RIGHT"  && canMoveLeft)
    {
        cerr << "move left" << endl;
        x--;
    }
    else
    {
        cerr << "from top" << endl;
        cerr << (canMoveRight ? "move left" : canMoveLeft ? " move left": "novhere") << endl;
        x = canMoveRight ? x + 1 : canMoveLeft ? x - 1 : x;
    }
    return x;
};


int main()
{
    //init
    int W; // number of columns.
    int H; // number of rows.
    cin >> W >> H; cin.ignore();
    //
    vector<vector<int>> greed = {};
    
    for (int i = 0; i < H; i++) {
        vector<int> row;
        string buf;
        string LINE; // represents a line in the grid and contains W integers. Each integer represents one room of a given type.
        getline(cin, LINE);
        stringstream ss(LINE);  
        
        while(ss >> buf)
            row.push_back(stoi(buf));
        
        greed.push_back(row);
    }
    
    int EX; // the coordinate along the X axis of the exit (not useful for this first mission, but must be read).
    cin >> EX; cin.ignore();

    // game loop
    while (1) {
        int XI;
        int YI;
        string POS;
        cin >> XI >> YI >> POS; cin.ignore();
        
        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;
        bool changed = false;
        if(YI + 1 < greed.size() &&
            canMoveFuther(greed[YI][XI], downBlock, 5))
        {
            if( ( greed[YI][XI] == 4 || greed[YI][XI] == 5 ) && POS == "TOP"){}
            else{
                cerr << "Move down" <<endl;
                changed = true;
                YI++;
            }
        }
        if(!changed)
        {
            cerr << "Move right or left" <<endl;
            XI = getX(greed, XI, YI, POS);
        }
        
        cout << to_string(XI) + " " + to_string(YI)  << endl; // One line containing the X Y coordinates of the room in which you believe Indy will be on the next turn.
    }
}