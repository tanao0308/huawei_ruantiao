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
    int get_x(){return x;}
    int get_y(){return y;}
    void print()
    {
        cerr<<"Robot: id="<<id<<", goods_num="<<goods_num<<", x="<<x<<", y="<<y<<endl;
        cerr<<"\tGoods = "<<gds.x<<" "<<gds.y<<" "<<gds.v<<" "<<gds.t<<endl;
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
    deque<int>op;
public:
    vector<RobotNorm*>*robot;
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
                int move = check_collision();
                if(move == -1) // 不会碰撞，则按原方案行动
                {
                    move = ber->robot_map[y][x];
                    cout<<"move "<<id<<" "<<move<<"\n";
                }
                else if(move == 4) // 无论如何都可能碰撞，则不动
                    ;
                else // 可以避障，则避障
                    cout<<"move "<<id<<" "<<move<<"\n";
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
                int move = check_collision();
                if(move==-1) // 不会碰撞，则按原方案行动
                {
                    move = get_movement();
                    cout<<"move "<<id<<" "<<move<<"\n";
                }
                else if(move == 4) // 无论如何都可能碰撞，则不动
                    ;
                else // 可以避障，则避障
                {
                    op.push_front(move^1);
                    cout<<"move "<<id<<" "<<move<<"\n";
                }
            }
        }
    }
    int check_collision() // 判断是否可能和其他机器人相撞，可能的话返回避让方向，不可能的话返回-1。如果无论如何避让都不能将距离拉到>=2则返回4
    {
        if(other_robot_dis(x,y)>=3)return -1;

        int randi[4]={0,1,2,3};random_shuffle(randi,randi+4);
        int f_move=-1,f_dis=0;
        for(int i=0;i<4;++i)
        {
            int move=randi[i];
            int new_x=x+dx[move],new_y=y+dy[move];
            int dis=other_robot_dis(new_x,new_y);
            if(dis>f_dis&&(Bfs_robot){new_x,new_y,0}.can_put())
                f_move=move,f_dis=dis;
        }
        if(f_dis>=2)return f_move;
        return 4;
    }
    int other_robot_dis(int x,int y) // 返回当前位置和最近的其他机器人的曼哈顿距离
    {
        int dis=1e9;
        for(int i=0;i<robot->size();++i)
        {
            if(i!=id&&priority()<robot->at(i)->priority())
                dis=min(dis,man_dis(x,y,robot->at(i)->get_x(),robot->at(i)->get_y()));
        }
        return dis;
    }
    int priority()
    {
        return id;
    }
    int get_movement()
    {
        if(op.empty())
            get_operation_queue();

        if(!op.empty())
        {
            int move = op.front();
            op.pop_front();
            return move;
        }
        else return -1;
    }
    int man_dis(int x,int y,int xx,int yy) {return abs(x-xx)+abs(y-yy);}
    void get_operation_queue()
    {
        stack<int>op_sta;
        while(!op.empty())op.pop_front();
        Bfs_robot g = {gds.x,gds.y,0};
        while(!ber->in_berth(g.x,g.y))
        {
            int move=ber->robot_map[g.y][g.x];
            if(move==-1)return;
            op_sta.push(move);
            g=g.move(move);
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
        }
        while(!op_sta.empty())
        {
            op.push_back(op_sta.top()^1);
            op_sta.pop();
        }
    }
};

#endif