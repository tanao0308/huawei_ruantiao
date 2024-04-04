#ifndef BOAT_CPP
#define BOAT_CPP

#include<bits/stdc++.h>
using namespace std;

class Boat {
protected:
    int id, x, y, dir;
    int goods_num, status;
    static constexpr int dx[4]={1,-1,0,0},dy[4]={0,0,-1,1};

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
    bool intersect(int x,int y)
    {
        if(abs(this->x-x)<=3||abs(this->y-y)<=3)return 1;
        for(int i=0;i<2;++i)
            for(int j=0;j<3;++j)
                if(this->x+j*dx[dir]==x&&this->y+i*dy[dir]==y)
                    return 1;
        return 0;
    }

    virtual void action() = 0;
};

class BoatNorm : public Boat {
public:
    ~BoatNorm() {}

    virtual void action() override 
    {
        int status = abs(rand()) % 2;
        if(status == 1)
            printf("ship %d\n", id);
        else
            printf("rot %d %d\n", id, rand() % 2);
    }
};

#endif