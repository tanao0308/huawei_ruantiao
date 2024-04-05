#ifndef BOAT_CPP
#define BOAT_CPP

#include<bits/stdc++.h>
#include "berth.cpp"
using namespace std;

class Boat {
protected:
    int id, x, y, dir;
    int goods_num, status;
    static constexpr int dx[4]={1,-1,0,0},dy[4]={0,0,-1,1};

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
    bool intersect(int tx,int ty)
    {
        int ox[4]={2,-2,1,-1},oy[4]={1,-1,-2,2};
        int lx=x,ly=y;
        int rx=lx+ox[dir],ry=ly+oy[dir];
        if(lx>rx)swap(lx,rx);
        if(ly>ry)swap(ly,ry);
        if(lx<=tx&&tx<=rx&&ly<=ty&&ty<=ry)return 1;
        return 0;
    }

    virtual void action() = 0;
};

class BoatNorm : public Boat {
private:
    Berth* ber;
    int load_time=0;
public:
    ~BoatNorm() {}

    virtual void action() override 
    {
        if(status==0) //正常行驶状态
        {
            if(goods_num) //当前船上有货物，则前往交货点
            {
                int operation = delivery_point[y][x][dir];
                if(operation == 2)cout<<"ship "<<id<<"\n";
                else cout<<"rot "<<id<<" "<<operation<<"\n";
            }
            else //当前船上无货物，则前往港口
            {
                if(ber->in_berth(x,y))
                    cout<<"berth "<<id<<"\n";
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
            continue;
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
        // int status = abs(rand()) % 2;
        // if(status == 1)
        //     printf("ship %d\n", id);
        // else
        //     printf("rot %d %d\n", id, rand() % 2);
    }
};

#endif