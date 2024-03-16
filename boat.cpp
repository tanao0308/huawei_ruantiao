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
    int get_berth0() {
        if(berth[id*2].transport_time>berth[id*2+1].transport_time)
            return id*2;
        else return id*2+1;
    }
    int get_action()
    {
        if(status==0)return -1;//在运行中，不操作
        if(tar==-1)//刚到虚拟点，去新目标
        {
            gds_num=0;
            tar=get_berth0();
            return tar+1;
        }
        else if(tar==get_berth0())//如果在第一个港口
        {
            if(t0==15000-berth[tar].transport_time-2)
            { //如果船在最后能出海的时间，那就让它出海
                tar=-1;
                return tar+1;
            }
            if(gds_num+berth[tar].loading_speed<=boat_capacity&&berth[tar].gds_num-berth[tar].loading_speed>=0)
            { //如果船在装货中，则装货
                gds_num+=berth[tar].loading_speed;
                berth[tar].gds_num-=berth[tar].loading_speed;
                return -1;
            }
            if(t0<15000-berth[tar].transport_time-2&&t0>=15000-500-berth[tar^1].transport_time-2-boat_capacity/berth[tar^1].loading_speed)
            { //如果船来得及在现在这个泊位出海但来不及在下一个泊位出海，则在此泊位等待
                return -1;
            }
            //不然，开往下一个泊位
            // if(gds_num+berth[tar].loading_speed>boat_capacity)cerr<<"boat capacity is too small"<<endl;
            // if(berth[tar].gds_num-berth[tar].loading_speed<0)cerr<<"goods is too few"<<endl;
            tar^=1;
            return tar+1;
        }
        else//如果在第二个港口
        {
            if(t0==15000-berth[tar].transport_time-2)
            { //如果船在最后能出海的时间，那就让它出海
                tar=-1;
                return tar+1;
            }
            if(gds_num+berth[tar].loading_speed<=boat_capacity&&berth[tar].gds_num-berth[tar].loading_speed>=0)
            { //如果船在装货中，则装货
                gds_num+=berth[tar].loading_speed;
                berth[tar].gds_num-=berth[tar].loading_speed;
                return -1;
            }
            if(t0<15000-berth[tar].transport_time-2&&t0>=15000-berth[tar].transport_time-berth[tar^1].transport_time-2-boat_capacity/berth[tar^1].loading_speed)
            { //如果船来得及在现在这个泊位出海但来不及在下一个泊位出海，则在此泊位等待
                return -1;
            }
            //不然，开往-1
            // if(gds_num+berth[tar].loading_speed>boat_capacity)cerr<<"boat capacity is too small"<<endl;
            // if(berth[tar].gds_num-berth[tar].loading_speed<0)cerr<<"goods is too few"<<endl;
            tar=-1;
            return tar+1;
        }
    }
};