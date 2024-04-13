#ifndef BOAT_CPP
#define BOAT_CPP

#include<bits/stdc++.h>
#include "lands.cpp"
using namespace std;

class Boat {
protected:
    int id, x, y, dir;
    int goods_num, status;

public:
    static int boat_capacity;
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
    bool intersect(int tx,int ty) //判断点(ty,tx)是否在船内部
    {
        int lx=x,ly=y;
        int rx=lx+bx[dir],ry=ly+by[dir];
        if(lx>rx)swap(lx,rx);
        if(ly>ry)swap(ly,ry);
        if(lx<=tx&&tx<=rx&&ly<=ty&&ty<=ry)return 1;
        return 0;
    }

    virtual void action() = 0;
};

class BoatNorm : public Boat {
private:
    int load_time=0;
    DeliveryPoint* del;
    vector<Berth*>cruise;int cruise_id=0;
public:
    ~BoatNorm() {}

    void set_cruise(vector<Berth*>&berth,DeliveryPoint*delivery_point)
    {
        cruise_id=0;
        for(int i=0;i<berth.size();++i)
            cruise.push_back(berth[i]);
        del = delivery_point;
    }
    virtual void action() override 
    {
        if(status==0) //正常行驶状态
        {
            if(cruise_id==cruise.size()) // 目前是准备前往交货点
            {
                if(in_DeliveryPoint()) // 如果现在到了交货点，则开始新周目
                {
                    cruise_id = (cruise_id+1)%(cruise.size()+1);
                }
                else // 否则，前往交货点
                {
                    int operation = del->boat_map[y][x][dir];
                    if(operation == 2)cout<<"ship "<<id<<"\n";
                    else cout<<"rot "<<id<<" "<<operation<<"\n";
                }
            }
            else // 否则，当前前往港口cruise[cruise_id]
            {
                Berth*ber=cruise[cruise_id];
                if(ber->in_berth(x,y)) // 如果现在到达了港口，则进港口，并设定下一个港口
                {
                    cout<<"berth "<<id<<"\n";
                    cruise_id = (cruise_id+1)%(cruise.size()+1);
                }
                else
                {
                    int operation = ber->boat_map[y][x][dir];
                    if(operation == 2)cout<<"ship "<<id<<"\n";
                    else cout<<"rot "<<id<<" "<<operation<<"\n";
                }
            }
        }
        else if(status==1) //恢复状态
        {
            return;
        }
        else //装载状态
        {
            if(load_time >= 100) //如果装载时间到了，就离港
            {
                cout<<"dept "<<id<<"\n";
                load_time=0;
            }
            else //否则装载时间累加
                load_time++;
        }
    }
    bool in_DeliveryPoint()
    {
        return intersect(del->get_x(),del->get_y());
    }
};

#endif