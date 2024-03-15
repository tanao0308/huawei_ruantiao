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
    int id,t0,pos,status;
    int t;
    void take_action()
    {
        int action=get_action();
        if(action==0)
            printf("go %d\n",id);
        else if(action!=-1)
            printf("ship %d %d\n",id,action-1);
    }
    int get_target()
    {
        return id*2+rand()%2;
    }
    int get_wait_time()
    {
        int max_wait_time=1000;
        return max_wait_time/berth[pos].loading_speed;
    }
    int get_action()
    {
        if(status==0)t=0;
        else t++;
        if(status==0)return -1;

        int target=get_target();
        if(pos==-1)return target+1;
        if(pos==target)
        {
            if(t>=get_wait_time())return 0;
            else return -1;
        }
        return -1;
    }
};