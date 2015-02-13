#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
int main()
{
    int LX; // the X position of the light of power
    int LY; // the Y position of the light of power
    int TX; // Thor's starting X position
    int TY; // Thor's starting Y position
    cin >> LX >> LY >> TX >> TY; cin.ignore();
    
    int vx = LX - TX;
    int vy = LY - TY;
    char dir_name[4] = {'N','E','S','W'};
    // game loop
    while (1) {
        int E; // The level of Thor's remaining energy, representing the number of moves he can still make.
        
        cin >> E; cin.ignore();
        
        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;
        cerr << to_string(vx) << " " << to_string(vy) << endl;
        
        char dir[2] = {'\0', '\0'};
        
        char *cd = &dir[0];
        
        if(vy != 0){
            *cd = vy > 0 ? dir_name[2] : dir_name[0];
            vy += vy > 0 ? -1 : 1;
           cd = &dir[1];
        }
        if(vx != 0){
            *cd = vx > 0 ? dir_name[1] : dir_name[3];
            vx += vx > 0 ? -1 : 1;
        }

        if(dir[1] != '\0')
            cout << dir[0] << dir[1] << endl; // A single line providing the move to be made: N NE E SE S SW W or NW
        else
            cout << dir[0] << endl;
    }
}
