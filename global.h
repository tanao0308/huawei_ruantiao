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
extern int id,money,boat_capacity;
extern char mp[200][200];
extern int mp_gds[200][200];
extern int mp_ber[200][200];

struct node {
    int x,y,s;
    bool walkable()
    {
        if(x<0||y<0||x>=n||y>=n)return 0;
        if(mp[y][x]!='.'&&mp[y][x]!='B'&&mp[y][x]!='A')return 0;
        return 1;
    }
};
struct Gds { //每个货物如未被拿到则1000帧后消失
    int x,y,t,v;
};
struct Berth
{
    int x;
    int y;
    int transport_time;
    int loading_speed;
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
        memset(dis,-1,sizeof dis);
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
            for(int i=0;i<4;++i)
            {
                node v=(node){u.x+dx[i],u.y+dy[i],u.s+1};
                if(!v.walkable()||route[v.y][v.x]!=-1)continue;
                dis[v.y][v.x]=v.s;
                route[v.y][v.x]=i^1;
                q.push(v);
            }
        }
    }

    queue<Gds>q;
    void update(int t)
    {
        while(!q.empty()&&t-q.front().t>1000-500)//50为参数
            q.pop();
    }
    Gds get_gds()
    {
        if(q.empty())return (Gds){-1,-1,-1,-1};
        Gds gds=q.front();
        q.pop();
        return gds;
    }
};
extern Berth berth[10];

#endif