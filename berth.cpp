#include<bits/stdc++.h>
using namespace std;

extern const int dx[4],dy[4];

class Berth
{
    int x, y;
    int loading_speed;
    Berth(){}
    Berth(int x, int y, int loading_speed) {
        this -> x = x;
        this -> y = y;
        this -> loading_speed = loading_speed;
    }
};