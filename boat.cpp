#ifndef BOAT_CPP
#define BOAT_CPP

#include<bits/stdc++.h>
using namespace std;

class Boat {
protected:
    int id, x, y, dir;
    int goods_num, status;
    static constexpr int dx[4]={0,0,-1,1},dy[4]={1,-1,0,0};

public:
    virtual ~Boat(){};

    void set(int id, int goods_num, int x, int y, int dir, int status)
    {
        this -> id = id;
        this -> goods_num = goods_num;
        this -> x = x;
        this -> y = y;
        this -> dir = dir;
        this -> status = status;
    }
    void print()
    {
        cerr<<"Boat: id="<<id<<", goods_num="<<goods_num<<", x="<<x<<", y="<<y<<", dir="<<dir<<", status="<<status<<endl;
    }

    virtual void action() = 0;
};

class BoatNorm : public Boat {
public:
    ~BoatNorm() {}

    virtual void action() override 
    {
    }
};

#endif