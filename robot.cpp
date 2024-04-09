#ifndef ROBOT_CPP
#define ROBOT_CPP

#include<bits/stdc++.h>
#include "lands.cpp"
using namespace std;

class Robot {
protected:
    int id, x, y;
    int goods_num;
    int status; //两种状态：返回港口和拿货，分别为0/1
    Goods gds;

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
        cerr<<"Goods = "<<gds.x<<" "<<gds.y<<" "<<gds.v<<" "<<gds.t<<endl;
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
    queue<int>op;
public:
    ~RobotNorm() {}
    RobotNorm() {
        this -> status = 0;
        this -> gds = {-1,-1,-1,-1};
    }

    void set_berth(Berth* ber)
    {
        this -> ber = ber;
    }
    virtual void action() override 
    {
        if(status==0) //返回港口
        {
            if(ber->in_berth(x,y))
            {
                if(goods_num)
                    cout<<"pull "<<id<<"\n";
                gds = ber->get_goods();
                status = 1;
            }
            else
            {
                int operation = ber->robot_map[y][x];
                cout<<"move "<<id<<" "<<operation<<"\n";
            }
        }
        else //去取货
        {
            if(x==gds.x&&y==gds.y)
            {
                cout<<"get "<<id<<"\n";
                gds = {-1,-1,-1,-1};
                status = 0;
            }
            else
            {
                int move = get_movement();
                if(move>=0&&move<4)
                    cout<<"move "<<id<<" "<<move<<"\n";
            }
        }
    }
    int man_dis(int x,int y,int xx,int yy) {return abs(x-xx)+abs(y-yy);}
    void get_operation_queue()
    {
        stack<int>op_sta;
        while(!op.empty())op.pop();
        Bfs_robot g = {gds.x,gds.y,0};
        while(!ber->in_berth(g.x,g.y))
        {
            int move=ber->robot_map[g.y][g.x];
            if(move==-1)return;
            op_sta.push(move);
            g=g.move(move);
            cerr<<"aaa"<<endl;
        }
        while(g.x!=x||g.y!=y)
        {
            int move=rand()%4;
            Bfs_robot g2=g.move(move);
            if(man_dis(g2.x,g2.y,x,y)<man_dis(g.x,g.y,x,y))
            {
                op_sta.push(move);
                g=g2;
            }
            cerr<<"bbb"<<endl;
        }
        while(!op_sta.empty())
        {
            op.push(op_sta.top()^1);
            op_sta.pop();
        }
    }
    int get_movement()
    {
        if(op.empty())
            get_operation_queue();

        if(!op.empty())
        {
            int move = op.front();
            op.pop();
            return move;
        }
        else return -1;
    }
};

#endif