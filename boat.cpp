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
extern Berth berth[10];

class BOAT
{
public:
    int id,t0,status;//船的编号，当前帧数，当前状态（0：运输中，1：正在装货/停止不动，2：泊位外等待）
    int gds_num;//当前货物个数
    int tar,gov_berth[2];//从-1到gov_berth[0]到gov_berth[1]到-1，tar表示当前目标的下标（-1或港口）
    void take_action()
    {
        int action=get_action();
        if(action==0)
            printf("go %d\n",id),cerr<<"go:"<<id<<", berth:"<<tar<<endl;
        else if(action!=-1)
            printf("ship %d %d\n",id,action-1);
    }
    bool is_start;
    int get_action()
    {
        // if(status==0)return -1;//在运行中，不操作
        int T=berth[gov_berth[0]].transport_time+boat_capacity/berth[gov_berth[0]].loading_speed+500+boat_capacity/berth[gov_berth[1]].loading_speed+berth[gov_berth[1]].transport_time;
        int left_time=T-(15000-t0)%T;//[1,T]范围
        if(left_time==1)
        {
            is_start=1;
            return gov_berth[0]+1;
        }
        if(is_start&&left_time==berth[gov_berth[0]].transport_time+boat_capacity/berth[gov_berth[0]].loading_speed)
            return gov_berth[1]+1;
        if(is_start&&left_time==berth[gov_berth[0]].transport_time+boat_capacity/berth[gov_berth[0]].loading_speed+500+boat_capacity/berth[gov_berth[1]].loading_speed)
        {
            // while(1);
            return 0;
        }
        return -1;

    }
};