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

class ROBOT
{
public:
    int id,t0,x,y;
    bool gds,status;// gds 0/1:手上有无货物，status 0/1:是否能动
    stack<int>op_sta;queue<int>op;queue<node>q;
    void take_action()
    {
        int pre_action=get_pre_action();
        if(pre_action==0)
            printf("get %d\n",id);
        else if(pre_action==1)
            printf("pull %d\n",id);

        int action=get_action();
        if(action>=0&&action<4)
            printf("move %d %d\n",id,action);
    }

    int get_berth(){return id;}//目标港口是和自己id相同的港口
    bool in_berth(int id,int x,int y)
    {
        if(x<berth[id].x||y<berth[id].y)return 0;
        if(x>=berth[id].x+4||y>=berth[id].y+4)return 0;
        return 1;
    }
    int get_pre_action()
    {
        if(!status)return -1;
        if(!op.empty())return -1;
        if(!in_berth(get_berth(),x,y))
        {
            if(mp_gds[y][x]&&!gds)
            {
                mp_gds[y][x]=0;
                gds=1;
                return 0;
            }
            else return -1;
        }
        else
        {
            if(gds){gds=0; return 1;}
            else return -1;
        }
    }
    int man_dis(int x,int y,int xx,int yy) {return abs(x-xx)+abs(y-yy);}
    void get_queue()
    {//route数组表示地图上某点按route走即可到港口
        int ber=get_berth();if(ber==-1)return;
        Gds gds=berth[ber].get_gds(t0);if(gds.x==-1)return;

        while(!op_sta.empty())op_sta.pop();
        while(!op.empty())op.pop();
        while(!in_berth(ber,gds.x,gds.y))
        {
            int action=berth[ber].route[gds.y][gds.x];
            if(action==-1)return;
            op_sta.push(action^1);
            gds.x+=dx[action];
            gds.y+=dy[action];
        }
        while(gds.x!=x||gds.y!=y)
        {
            int action=rand()%4;
            Gds gds2=(Gds){gds.x+dx[action],gds.y+dy[action],-1,-1};
            if(man_dis(gds2.x,gds2.y,x,y)<man_dis(gds.x,gds.y,x,y))
            {
                op_sta.push(action^1);
                gds=gds2;
            }
        }
        while(!op_sta.empty())
        {
            op.push(op_sta.top());
            op_sta.pop();
        }
    }
    bool walkable(int x,int y)
    {
        if(x<0||y<0||x>=n||y>=n)return 0;
        if(mp[y][x]!='.'&&mp[y][x]!='B')return 0;
        return 1;
    }
    int get_action()
    {
        if(!op.empty()) {//有操作序列那么按操作序列做
            int res=op.front();
            op.pop();
            return res;
        }
        //以下是没操作序列的情况
        if(in_berth(get_berth(),x,y)) {//如果当前在港口（则当前手上必然是空的）
            get_queue();
            if(op.empty())//目前港口所在的区域没有物体能拿，就休息一会儿
                return -1;
            else
            {
                int res=op.front();
                op.pop();
                return res;
            }
        }
        else {//如果当前不在港口且没有操作队列，那就要返回港口，无论手上有没有东西
            int ber=get_berth();
            // if(berth[ber].route[y][x]!=-1)while(1);
            return berth[ber].route[y][x];
        }
    }
};