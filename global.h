#ifndef GLOBAL_H
#define GLOBAL_H

#include <bits/stdc++.h>
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
extern int mp_ber[200][200];

struct node {
    int x,y,s;
    bool walkable()
    {
        if(x<0||y<0||x>=n||y>=n)return 0;
        if(mp[y][x]!='.'&&mp[y][x]!='B')return 0;
        return 1;
    }
};
struct Gds { //每个货物如未被拿到则1000帧后消失
    int x,y,t,v,d;//位置、生成时间、价值、离最近港口距离
    bool operator<(const Gds b)const {
        return (double)v/d<(double)b.v/b.d;
    }
    bool reachable(int t0)const {
        return t+1000-t0+2>d;
    }
};
struct Berth
{
    int x,y;
    int transport_time;
    int loading_speed;
    int gds_num;
    queue<int>q_boat;
    int total_gds_value;
    Berth(){}
    Berth(int x, int y, int transport_time, int loading_speed) {
        this -> x = x;
        this -> y = y;
        this -> transport_time = transport_time;
        this -> loading_speed = loading_speed;
    }
    
    int route[200][200]; //从网格上任何一点到达当前泊位的一条最短路径
    int dis[200][200];
    void get_route()
    {
        queue<node>q;
        memset(dis,0x3f,sizeof dis);
        memset(route,-1,sizeof route);
        for(int i=0;i<4;++i)
            for(int j=0;j<4;++j)
            {
                dis[y+i][x+j]=0;
                route[y+i][x+j]=4;
                q.push((node){x+j,y+i,0});
            }
        while(!q.empty())
        {
            node u=q.front();q.pop();
            int randi[4]={0,1,2,3};random_shuffle(randi,randi+4);
            for(int ii=0;ii<4;++ii)
            {
                int i=randi[ii];
                node v=(node){u.x+dx[i],u.y+dy[i],u.s+1};
                if(!v.walkable()||route[v.y][v.x]!=-1)continue;
                dis[v.y][v.x]=v.s;
                route[v.y][v.x]=i^1;
                q.push(v);
            }
        }
    }

    priority_queue<Gds>q;
    Gds get_gds(int t0)
    {
        while(!q.empty()&&!q.top().reachable(t0))q.pop();
        if(q.empty())return (Gds){-1,-1,-1,-1,-1};
        Gds gds=q.top();q.pop();
        return gds;
    }
};
extern Berth berth[10];

struct RobotData {
    int x,y,pri;
};
extern RobotData robot_data[10];

#endif