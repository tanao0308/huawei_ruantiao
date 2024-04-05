#ifndef ROBOT_CPP
#define ROBOT_CPP

#include<bits/stdc++.h>
#include "lands.cpp"
using namespace std;

class Robot {
protected:
    int id, x, y;
    int goods_num;

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
    bool intersect(int x,int y) //判断点(ty,tx)是否和机器人重合
    {
        if(this->x==x&&this->y==y)
            return 1;
        return 0;
    }

    virtual void action() = 0;
};

class RobotNorm : public Robot {
private:
    Berth* ber;
public:
    ~RobotNorm() {}

    void set_berth(Berth* ber)
    {
        this -> ber = ber;
    }
    virtual void action() override 
    {
        if(goods_num)
        {
            if(ber->in_berth(x,y))
                cout<<"pull "<<id<<"\n";
            else
            {
                int operation = ber->robot_map[y][x];
                cout<<"move "<<id<<" "<<operation<<"\n";
            }
        }
        else
        {
            cout<<"move "<<id<<" "<<rand() % 4<<"\n";
            cout<<"get "<<id<<"\n";
        }
    }
};

#endif