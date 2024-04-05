#ifndef BERTH_CPP
#define BERTH_CPP

#include<bits/stdc++.h>
using namespace std;

class Berth {
private:
    int id, lx, ly, rx, ry;
    int loading_speed;
    int boat_map[200][200][4];
    int robot_map[200][200];
public:
    Berth(){}
    Berth(int id, int lx, int ly, int rx, int ry, int loading_speed) {
        this -> id = id;
        this -> lx = lx;
        this -> ly = ly;
        this -> rx = rx;
        this -> ry = ry;
        this -> loading_speed = loading_speed;
    }
    void print()
    {
        cerr<<"Berth: id="<<id<<", lx="<<lx<<", ly="<<ly<<", rx="<<rx<<", ry="<<ry<<", loading_speed="<<loading_speed<<endl;
    }
    bool in_berth(int x,int y)
    {
        if(lx<=x&&x<=rx&&ly<=y&&y<=ry)
            return 1;
        return 0;
    }
    void init()
    {
        
    }
};

#endif