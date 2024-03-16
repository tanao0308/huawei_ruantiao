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
int t0,money,boat_capacity;
char mp[200][200];
int mp_gds[200][200];
int mp_ber[200][200];
Berth berth[10];
RobotData robot_data[10];

ROBOT robot[10];
BOAT boat[5];

int select_k()
{
    return 4;
}
void get_mp_ber()
{
    memset(mp_ber,-1,sizeof mp_ber);
    int sum_dis[11]={0};
    for(int i=0;i<n;++i)
        for(int j=0;j<n;++j)
        {
            int b0=-1;
            for(int b=0;b<berth_num;++b)
            {
                if(berth[b].dis[i][j]>=1e9)continue;
                if(b0==-1||berth[b].dis[i][j]<berth[b0].dis[i][j])
                    b0=b;
            }
            if(b0==-1)continue;
            sum_dis[b0]+=berth[b0].dis[i][j];
            sum_dis[10]+=berth[b0].dis[i][j];
        }
    int a[10]={0};for(int i=0;i<10;++i)a[i]=sum_dis[i];
    sort(a,a+10);int k=select_k();
    bool use_berth[10]={0};
    for(int i=0;i<berth_num;++i)
        if(sum_dis[i]>=a[k]) //表示取前10-k大的港口
            use_berth[i]=1;
    // 以下为弃置方案
    // double k=0.2;//0.3map1:19.5w 0.4map1:20w 0.5map1:19.1w 0.6map1:19w
    // bool use_berth[10]={0};
    // for(int i=0;i<berth_num;++i)
    //     if(sum_dis[i]>0.1*sum_dis[10]*k) //表示如果管辖范围大于某个值则启用这个港口
    //         use_berth[i]=1;

    cerr<<"-------------------------------"<<endl;
    cerr<<"sum dis="<<sum_dis[10]<<endl;
    cerr<<a[k]<<"||"<<166666<<endl;
    for(int i=0;i<=berth_num;++i)cerr<<sum_dis[i]<<" ";cerr<<endl;
    // while(1);
    
    //现在已知港口的启用情况，接下来给港口分配管辖区域和机器人数量
    memset(sum_dis,0,sizeof sum_dis);
    for(int i=0;i<n;++i)
        for(int j=0;j<n;++j)
        {
            int b0=-1;
            for(int b=0;b<berth_num;++b)if(use_berth[b])
            {
                if(berth[b].dis[i][j]>=1e9)continue;
                if(b0==-1||berth[b].dis[i][j]<berth[b0].dis[i][j])
                    b0=b;
            }
            if(b0==-1)continue;
            mp_ber[i][j]=b0;
            sum_dis[b0]+=berth[b0].dis[i][j];
            sum_dis[10]+=berth[b0].dis[i][j];
        }

    cerr<<"-------------------------------"<<endl;
    for(int i=0;i<berth_num;++i)cerr<<use_berth[i]<<" ";cerr<<endl;
    for(int i=0;i<berth_num;++i)cerr<<sum_dis[i]<<" ";cerr<<endl;
    // while(1);
    

    int p=0;//当前准备分配的机器人id
    queue<int>use_robot;int work_robot=0;
    for(int i=0;i<robot_num;++i)
        if(mp_ber[robot[i].y][robot[i].x]!=-1)
            use_robot.push(i),work_robot++;
    while(!use_robot.empty())
    {
        int b0=-1;
        for(int b=0;b<berth_num;++b)if(use_berth[b])
            if(b0==-1||sum_dis[b0]<sum_dis[b])
                b0=b;
        if(b0==-1)continue;
        sum_dis[b0]-=(1.0/work_robot)*sum_dis[10];
        robot[use_robot.front()].berth_id=b0;

        cerr<<use_robot.front()<<" ";
        use_robot.pop();
    }cerr<<endl;
    // while(1);

    // for(int i=0;i<robot_num;++i)
    //     cerr<<robot[i].get_berth()<<" ";cerr<<endl;
    // while(1);
}
void print_map()
{
    int watch=9;
    cerr<<endl;
    for(int i=0;i<n;++i)
    {
        for(int j=0;j<n;++j)
            if(mp_ber[i][j]==-1)cerr<<" X";
            else if(mp_ber[i][j]==watch)cerr<<" "<<'O';
            else cerr<<" .";
            // else cerr<<" .";
        cerr<<endl;
    }
    while(1);
}
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
    }
    scanf("%d",&boat_capacity);
    char okk[100];
    scanf("%s",okk);
    printf("OK\n");
    fflush(stdout);

    for(int i=0;i<berth_num;++i)
        berth[i].get_route();
}
void Input()
{
    scanf("%d%d",&t0,&money); //当前帧编号，当前钱总数

    int gds_num;
    scanf("%d",&gds_num); //当前新增货物总数
    for(int i=0;i<gds_num;++i)
    {
        int x,y,v;
        scanf("%d%d%d",&y,&x,&v); //新增货物的位置和价值
        int ber=mp_ber[y][x];if(ber==-1)continue;
        berth[ber].q.push((Gds){x,y,t0,v,berth[ber].dis[y][x]});
        mp_gds[y][x]=v;
    }
    for(int i=0;i<robot_num;i++)
    {
        robot[i].id=i,robot[i].t0=t0;
        scanf("%d%d%d%d",&robot[i].gds,&robot[i].y,&robot[i].x,&robot[i].status); //当前机器人是否拿着货物、位置、状态
        robot_data[i].x=robot[i].x,robot_data[i].y=robot[i].y;

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
}
int main()
{
    Init();
    for(int zhen=1;zhen<=15000;zhen++)
    {
        Input();
        if(zhen==1)
        {
            get_mp_ber();
            // print_map();
        }
        Action();
        puts("OK");
        fflush(stdout);
    }
    return 0;
}
