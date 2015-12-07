#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>

using namespace std;

class MayanDict{
private:
    int length;
    int height;
    map<string, int> dict;
    string getKey(vector<string> &, int, int);
    string getValue(int);
public:
    MayanDict(vector<string> &, int);
    ~MayanDict();
    string encode(unsigned long int);
    int decode(string);
};
/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
int main()
{
    int L;
    int H;
    vector<string> numeralStrings;
    cin >> L >> H; cin.ignore();
    for (int i = 0; i < H; i++) {
        string numeral;
        cin >> numeral; cin.ignore();
        numeralStrings.push_back(numeral);
    }
    //initialize dict
    MayanDict dict = MayanDict(numeralStrings, L);
    //start calculation
    unsigned long int S1;
    string num1Line;
    cin >> S1; cin.ignore();
    for (int i = 0; i < S1; i++) {
        string part;
        cin >> part; cin.ignore();
        num1Line += part;
    }
    S1 = dict.decode(num1Line);
    unsigned long int S2;
    string num2Line;
    cin >> S2; cin.ignore();
    for (int i = 0; i < S2; i++) {
        string part;
        cin >> part; cin.ignore();
        num2Line += part;
    }
    S2 = dict.decode(num2Line);
    char operation;
    cin >> operation; cin.ignore();
    
    unsigned long int result;
    switch(operation)
    {
        case '+':result = S1 + S2;break;
        case '-':result = S1 - S2;break;
        case '*':result = S1 * S2;break;
        case '/':result = S1 / S2;break;
    }
    // Write an action using cout. DON'T FORGET THE "<< endl"
    // To debug: cerr << "Debug messages..." << endl;
    string number = dict.encode(result);
    for(int i = 0; i < number.size(); i += L)
    {
        cout << number.substr(i, L) << endl;
    }
}

MayanDict::MayanDict(vector<string> & numerals, int len)
{
    length = len;
    height = numerals.size();
    int n = numerals[0].size() / len;
    for(int i = 0; i < n; ++i)
    {
        string key = getKey(numerals, len, i * len);
        dict[key] = i;
    }
}

string MayanDict::getKey(vector<string> & numerals, int len , int pos)
{
    string result;
    for(int i = 0; i < numerals.size(); ++i)
    {
        string numeral = numerals.at(i);
        for(int j = pos; j < pos + len; ++j)
            result += numeral.at(j);
    }
    return result;
}

MayanDict::~MayanDict(){}

int MayanDict::decode(string value)
{
    int result;
    int size = length * height;
    int tokensLen = value.size() / size;
    //  cerr << tokensLen << endl;
    for(int i = 0; i < tokensLen; ++i)
    {
        auto token = value.substr(size * i, size);
        auto it = dict.find(token);
        result += it->second * pow(20, (tokensLen - 1) - i);
    }
    return result;
}

string MayanDict::encode(unsigned long int value)
{
    string result;
    //
    if(value == 0)
        result = getValue(value);
    while(value > 0)
    {
        result = getValue(value % 20) + result;//as lower bit
        value /= 20;
    }
    return result;
}

string MayanDict::getValue(int value)
{
    string result;
    for(auto it = dict.begin(); it != dict.end(); ++it)
    {
        if(it->second == value)
        {
            result = it->first;   
            break;
        }
    }
    
    return result;
}