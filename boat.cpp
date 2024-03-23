#include<bits/stdc++.h>
#include "global.h"
using namespace std;

extern int t0,boat_capacity;
extern Berth berth[10];

class BOAT
{
public:
    int id,t0,status;
    int gds_num;
    int tar,gov_berth[2];
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
        int T=berth[gov_berth[0]].transport_time+boat_capacity/berth[gov_berth[0]].loading_speed+500+boat_capacity/berth[gov_berth[1]].loading_speed+berth[gov_berth[1]].transport_time;
        int left_time=T-(15000-t0)%T;
        if(15000-t0<T&&left_time<=berth[gov_berth[0]].transport_time+boat_capacity/berth[gov_berth[0]].loading_speed)
            berth[gov_berth[0]].robots=999,berth[gov_berth[1]].robots-=1;
        
        if(left_time==1)
        {
            is_start=1;
            return gov_berth[0]+1;
        }
        if(is_start&&left_time==berth[gov_berth[0]].transport_time+boat_capacity/berth[gov_berth[0]].loading_speed)
            return gov_berth[1]+1;
        if(is_start&&left_time==berth[gov_berth[0]].transport_time+boat_capacity/berth[gov_berth[0]].loading_speed+500+boat_capacity/berth[gov_berth[1]].loading_speed)
            return 0;
        return -1;

    }
};