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
int t0,money,boat_capacity,all_gds_val;
char mp[200][200];
Berth berth[10];
Gds exist_gds[200][200];
RobotData robot_data[10];
ROBOT robot[10];
BOAT boat[5];

void Init()
{
    for(int i=0;i<n;i++)
        scanf("%s",mp[i]);
    for(int i=0;i<n;++i)
        for(int j=0;j<n;++j)
            if(mp[i][j]=='A')
                mp[i][j]='.';
    for(int i=0;i<berth_num;i++)
    {
        int id;
        scanf("%d",&id);
        scanf("%d%d%d%d",&berth[id].y,&berth[id].x,&berth[id].transport_time,&berth[id].loading_speed);
        cerr<<berth[id].transport_time<<" ";
        // cerr<<berth[id].loading_speed<<" ";
    }
    // while(1);
    scanf("%d",&boat_capacity);
    // cerr<<boat_capacity<<endl;while(1);
    char okk[100];
    scanf("%s",okk);
    printf("OK\n");
    fflush(stdout);

    for(int i=0;i<berth_num;++i)
        berth[i].get_route();
    for(int i=0;i<robot_num;++i)
        robot[i].target_berth=-1;
}
void Input()
{

    scanf("%d%d",&t0,&money); //当前帧编号，当前钱总数

    for(int i=0;i<n;++i)
        for(int j=0;j<n;++j)
            if(exist_gds[i][j].t+1000<t0)
                exist_gds[i][j].t=0;

    int gds_num;
    scanf("%d",&gds_num); //当前新增货物总数
    for(int i=0;i<gds_num;++i)
    {
        int x,y,v;
        scanf("%d%d%d",&y,&x,&v); //新增货物的位置和价值
        exist_gds[y][x]={x,y,t0,v,0};
    }
    for(int i=0;i<robot_num;i++)
    {
        robot[i].id=i,robot[i].t0=t0;
        scanf("%d%d%d%d",&robot[i].gds,&robot[i].y,&robot[i].x,&robot[i].status); //当前机器人是否拿着货物、位置、状态
        robot_data[i].x=robot[i].x,robot_data[i].y=robot[i].y,robot_data[i].pri=robot[i].get_priority();

        // if(robot[i].status==0)while(1)cerr<<"collision!"<<endl;
    }
    for(int i=0;i<boat_num;i++)
    {
        boat[i].id=i,boat[i].t0=t0;
        scanf("%d%d\n", &boat[i].status, &boat[i].tar); //当前船
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
    cerr<<"All select goods value:"<<all_gds_val<<endl;
    for(int i=0;i<10;++i)cerr<<berth[i].total_gds_value<<" ";cerr<<endl;
    for(int i=0;i<10;++i)cerr<<berth[i].transport_time<<" ";cerr<<endl;
    for(int i=0;i<10;++i)cerr<<(double)berth[i].total_gds_value/berth[i].transport_time<<" ";cerr<<endl;
}
int main()
{
    Init();
    for(int zhen=1;zhen<=15000;zhen++)
    {
        Input();
        Action();
        puts("OK");

        // if(zhen==15000)

        fflush(stdout);
    }
    return 0;
}
