#ifndef ROBOT_CPP
#define ROBOT_CPP

#include<bits/stdc++.h>
using namespace std;

class Robot {
protected:
    int id, x, y;
    int goods_num;
    static constexpr int dx[4]={0,0,-1,1},dy[4]={1,-1,0,0};

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

    virtual void action() = 0;
};

class RobotNorm : public Robot {
public:
    ~RobotNorm() {}

    virtual void action() override 
    {
    }
};

#endif