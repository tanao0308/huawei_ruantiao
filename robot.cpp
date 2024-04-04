#ifndef ROBOT_CPP
#define ROBOT_CPP

#include<bits/stdc++.h>
using namespace std;

class Robot {
protected:
    int id, x, y;
    int goods_num;
    static constexpr int dx[4]={1,-1,0,0},dy[4]={0,0,-1,1};

public:
    virtual ~Robot(){};

    void set(int id, int goods_num, int x, int y)
    {
        this -> id = id;
        this -> goods_num = goods_num;
        this -> x = x;
        this -> y = y;
    }
    void print()
    {
        cerr<<"Robot: id="<<id<<", goods_num="<<goods_num<<", x="<<x<<", y="<<y<<endl;
    }
    bool intersect(int x,int y)
    {
        if(this->x==x&&this->y==y)
            return 1;
        return 0;
    }

    virtual void action() = 0;
};

class RobotNorm : public Robot {
public:
    ~RobotNorm() {}

    virtual void action() override 
    {
        printf("move %d %d\n", id, rand() % 4);
    }
};

#endif