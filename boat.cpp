#include<bits/stdc++.h>
#include "global.h"
using namespace std;

extern const int n;
extern const int N;
extern const int robot_num;
extern const int boat_num;
extern const int berth_num;
extern const int dx[4],dy[4];
extern int t0,money,boat_capacity;
extern char mp[200][200];
extern int mp_gds[200][200];
extern Berth berth[10];

class BOAT
{
public:
    int id,t0,tar,status;//船的编号，当前帧数，当前目标（-1或港口），当前状态（0：运输中，1：正在装货/停止不动，2：泊位外等待）
    int gds_num;//当前货物个数
    void take_action()
    {
        int action=get_action();
        if(action==0)
            printf("go %d\n",id);
        else if(action!=-1)
            printf("ship %d %d\n",id,action-1);
    }
    int get_target() {
        int ber=-1;
        for(int i=0;i<berth_num;++i)
        {
            if(!berth[i].q_boat.empty())
                continue;
            if(ber==-1||berth[ber].gds_num<berth[i].gds_num)
                ber=i;
        }
        if(ber==-1)return -1;
        berth[ber].q_boat.push(id);
        return ber;
        // return id*2+rand()%2;
    }
    int get_action()
    {
        if(status==0)return -1;//在运行中，不操作
        if(tar==-1)//刚到虚拟点，去新目标
        {
            gds_num=0;
            tar=get_target();
            return tar+1;
        }
        else//不然，就是在港口
        {
            if(gds_num+berth[tar].loading_speed<=boat_capacity&&berth[tar].gds_num-berth[tar].loading_speed>=0)//在装货中
            {
                gds_num+=berth[tar].loading_speed;
                berth[tar].gds_num-=berth[tar].loading_speed;
                return -1;
            }
            else
            {
                if(gds_num+berth[tar].loading_speed>boat_capacity)cerr<<"boat capacity is too small"<<endl;
                if(berth[tar].gds_num-berth[tar].loading_speed<0)cerr<<"goods is too few"<<endl;
                berth[tar].q_boat.pop();
                tar=-1;
                return tar+1;
            }
        }
    }
};