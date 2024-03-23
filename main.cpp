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
int mp_gds[200][200];
int mp_ber[200][200];
Berth berth[10];
RobotData robot_data[10];

ROBOT robot[10];
BOAT boat[5];

int MAP;
int select_k()
{
    if(MAP==1)
        return 3;
    if(MAP==2)
        return 4;
    if(MAP==3)
        return 2;
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
    for(int b=0;b<10;++b)
    {
        if(MAP!=1)
            sum_dis[b]=sqrt(sum_dis[b]);
        else
            sum_dis[b]=pow(sum_dis[b],1);
        sum_dis[10]+=sum_dis[b];
    }
    

    int p=0;//当前准备分配的机器人id
    bool workable[10]={0};int work_robot=0;//每个机器人是否可用，以及可用机器人的个数
    for(int i=0;i<robot_num;++i)
        if(mp_ber[robot[i].y][robot[i].x]!=-1)
            workable[i]=1,work_robot++;
    for(int tt=0;tt<100;++tt)
    {
        int b0=-1;
        for(int b=0;b<berth_num;++b)if(use_berth[b])
            if(b0==-1||sum_dis[b0]<sum_dis[b])
                b0=b;
        if(b0==-1)continue;
        //选出了当前需要分配机器人的港口
        sum_dis[b0]-=(1.0/work_robot)*sum_dis[10];
        for(int i=0;i<10;++i)
            if(berth[b0].route[robot[i].y][robot[i].x]!=-1&&workable[i])
            {//如果i号机器人有路径到b且没被选过
                workable[i]=0;
                robot[i].berth_id=b0;
                break;
            }
    }
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

    if(mp[87][13]=='B'&&mp[90][16]=='B')//map1
        MAP=1;
    else if(mp[32][175]=='B'&&mp[35][178]=='B')//map2
        MAP=2;
    else MAP=3;
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
        robot_data[i].x=robot[i].x,robot_data[i].y=robot[i].y;robot_data[i].pri=robot[i].get_priority();
    }
    for(int i=0;i<boat_num;i++)
    {
        boat[i].id=i,boat[i].t0=t0;
        scanf("%d%d\n", &boat[i].status, &boat[i].tar); //当前船
    }
    char okk[100];
    scanf("%s",okk);
}

int waste_value(int a,int b)//-1 -> a -> b -> -1的浪费值
{
    int ta=berth[a].transport_time,tb=berth[b].transport_time;
    int va=berth[a].total_gds_value,vb=berth[b].total_gds_value;
    int la=boat_capacity/berth[a].loading_speed,lb=boat_capacity/berth[b].loading_speed;
    return va*(500+lb+tb)+vb*tb;
}
int all_waste_value(vector<int>x)
{
    if(!x.size())return 1e9;
    int sum=0;
    for(int i=0;i<5;++i)
    {
        int a=x[i*2],b=x[i*2+1];
        sum+=min(waste_value(a,b),waste_value(b,a));
    }
    return sum;
}
vector<int>boat_berth,best_boat_berth;bool vis[10];
void dfs()
{
    if(boat_berth.size()==10)
    {
        if(all_waste_value(boat_berth)<all_waste_value(best_boat_berth))
            best_boat_berth=boat_berth;
        return;
    }
    for(int i=0;i<10;++i)
        for(int j=i+1;j<10;++j)if(!vis[i]&&!vis[j])
        {
            vis[i]=1,vis[j]=1;
            boat_berth.push_back(i),boat_berth.push_back(j);
            dfs();
            boat_berth.pop_back(),boat_berth.pop_back();
            vis[i]=0,vis[j]=0;
        }
}
void assign_boat_berth()
{
    dfs();
    for(int i=0;i<5;++i)
    {
        int a=best_boat_berth[i*2],b=best_boat_berth[i*2+1];
        if(waste_value(a,b)>waste_value(b,a))swap(a,b);
        boat[i].gov_berth[0]=a,boat[i].gov_berth[1]=b;
    }
}
void Action()
{
    for(int i=0;i<robot_num;++i)
        robot[i].take_action();
#define PRE_TIME 500
    if(t0==PRE_TIME)
    {
        assign_boat_berth();
        for(int i=0;i<boat_num;++i)
            boat[i].tar=-1;
    }
    if(t0>PRE_TIME)
    {
        for(int i=0;i<boat_num;++i)
            boat[i].take_action();
    }
    if(t0>=14990)
    {
        cerr<<"All select goods value:"<<all_gds_val<<endl;
        for(int i=0;i<10;++i)cerr<<berth[i].total_gds_value<<" ";cerr<<endl;
        for(int i=0;i<10;++i)cerr<<berth[i].transport_time<<" ";cerr<<endl;
        for(int i=0;i<10;++i)cerr<<(double)berth[i].total_gds_value/berth[i].transport_time<<" ";cerr<<endl;
        for(int i=0;i<5;++i)cerr<<boat[i].gov_berth[0]<<" ";cerr<<endl;
        for(int i=0;i<5;++i)cerr<<boat[i].gov_berth[1]<<" ";cerr<<endl;
    }
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
