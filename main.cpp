#include <bits/stdc++.h>
#include "global.h"
#include "robot.cpp"
#include "boat.cpp"
using namespace std;

const int n = 200;
const int N = 210;
const int robot_num = 10;
const int boat_num = 5;
const int berth_num = 10;
const int dx[4]={1,-1,0,0},dy[4]={0,0,-1,1};
int id,money,boat_capacity;
char mp[200][200];
int mp_gds[200][200];
int route[200][200];
int mp_ber[200][200];
Berth berth[10];

ROBOT robot[10];
BOAT boat[5];


void get_route()
{
    queue<node>q;
    memset(route,-1,sizeof route);
    for(int i=0;i<berth_num;++i)
    {
        route[berth[i].y+1][berth[i].x+1]=4;
        q.push((node){berth[i].x+1,berth[i].y+1,0});
    }
    while(!q.empty())
    {
        node u=q.front();q.pop();
        for(int i=0;i<4;++i)
        {
            node v=(node){u.x+dx[i],u.y+dy[i],u.s+1};
            if(!v.walkable()||route[v.y][v.x]!=-1)continue;
            route[v.y][v.x]=i^1;
            q.push(v);
        }
    }
}
void Init()
{
    for(int i=0;i<n;i++)
        scanf("%s",mp[i]);
    for(int i=0;i<berth_num;i++)
    {
        scanf("%d",&id);
        scanf("%d%d%d%d",&berth[id].y,&berth[id].x,&berth[id].transport_time,&berth[id].loading_speed);
    }
    scanf("%d",&boat_capacity);
    char okk[100];
    scanf("%s",okk);
    printf("OK\n");
    fflush(stdout);

    get_route();
    for(int i=0;i<berth_num;++i)
        berth[i].get_route();
    for(int i=0;i<n;++i)
        for(int j=0;j<n;++j)
        {
            int b0=-1;
            for(int b=1;b<berth_num;++b)
            {
                if(berth[b].dis[i][j]==-1)continue;
                if(b0==-1||berth[b].dis[i][j]<berth[b0].dis[i][j])
                    b0=b;
            }
            mp_ber[i][j]=b0;
        }
}
void Input()
{
    scanf("%d%d",&id,&money); //当前帧编号，当前钱总数

    for(int i=0;i<berth_num;++i)
        berth[i].update(id);

    int gds_num;
    scanf("%d",&gds_num); //当前新增货物总数
    for(int i=0;i<gds_num;++i)
    {
        int x,y,v;
        scanf("%d%d%d",&y,&x,&v); //新增货物的位置和价值
        berth[mp_ber[y][x]].q.push((Gds){x,y,id,v});
        mp_gds[y][x]=v;
    }
    for(int i=0;i<robot_num;i++)
    {
        robot[i].id=i;
        scanf("%d%d%d%d",&robot[i].gds,&robot[i].y,&robot[i].x,&robot[i].status); //当前机器人是否拿着货物、位置、状态
    }
    for(int i=0;i<boat_num;i++)
    {
        boat[i].id=i;
        scanf("%d%d\n", &boat[i].status, &boat[i].pos); //当前船
    }
    char okk[100];
    scanf("%s",okk);
}
void Action()
{
    for(int i=0;i<robot_num;++i)
        robot[i].take_action();
    for(int i=0;i<boat_num;++i)
        boat[i].take_action();
}
int main()
{
    Init();
    for(int zhen=1;zhen<=15000;zhen++)
    {
        Input();
        // if(zhen<=1000)
        {
            Action();
        }
        puts("OK");
        fflush(stdout);
    }
    return 0;
}