#include <bits/stdc++.h>
#include "boat.cpp"
#include "robot.cpp"
#include "berth.cpp"
#include "purchase.cpp"
using namespace std;
const int N = 200;

int robot_num = 0;
int boat_num = 0;
int berth_num = 0;
vector<Boat*>boat;
vector<Robot*>robot;
vector<Berth*>berth;

int goods_num = 0;
int frame_id = 0;
int money, boat_capacity;
int Boat::boat_capacity = 0;

char grid[N][N];

vector<RobotPurchasePoint*> robot_purchase_point;
vector<BoatPurchasePoint*> boat_purchase_point;
vector<pair<int, int>> delivery_point;
void ProcessMap()
{
    for(int i = 0; i < N; i ++){
        for(int j = 0; j < N; j ++){
            if(grid[i][j] == 'R')
            {
                RobotPurchasePoint* rpp = new RobotPurchasePoint(j,i);
                robot_purchase_point.push_back(rpp);
                // robot_purchase_point.back()->print();
            }
            else if(grid[i][j] == 'S')
            {
                BoatPurchasePoint* bpp = new BoatPurchasePoint(j,i);
                boat_purchase_point.push_back(bpp);
                // boat_purchase_point.back()->print();
            }
            else if(grid[i][j] == 'T')
                delivery_point.push_back(make_pair(i, j));
        }
    }
}
void Init()
{
    for(int i = 0; i < N; i ++){
        cin>>grid[i];
    }
    ProcessMap();
    cin>>berth_num;
    for(int i = 0; i < berth_num; i ++)
    {
        int id, x, y, loading_speed;
        cin>>id>>y>>x>>loading_speed;

        int lx=x,ly=y,rx=x,ry=y;
        while(grid[ly][lx-1]=='B')lx--;
        while(grid[ly-1][lx]=='B')ly--;
        while(grid[ry][rx+1]=='B')rx++;
        while(grid[ry+1][rx]=='B')ry++;
        Berth* ber = new Berth(id,lx,ly,rx,ry,loading_speed);
        // ber->print();
        berth.push_back(ber);
    }
    cin>>boat_capacity;
    Boat::boat_capacity = boat_capacity;
    for(int i=0;i<berth.size();++i)
        berth[i]->init();

    char okk[100];
    cin>>okk;
    printf("OK\n");
    fflush(stdout);
}
void Input()
{
    cin>>money;

    cin>>goods_num;
    for(int i = 0; i < goods_num; i ++)
    {
        int x, y, val;
        cin>>y>>x>>val;
        if(val == 0) val = 0;
    }

    cin>>robot_num;cerr<<robot_num<<"robot"<<robot.size()<<endl;//assert(robot_num == (int)robot.size());
    for(int i = 0; i < robot_num; i ++)
    {
        int id, goods_num, x, y;
        cin>>id>>goods_num>>y>>x;
        robot[i]->set(id, goods_num, x, y);
        robot[i]->print();
    }

    cin>>boat_num;cerr<<boat_num<<"boat"<<boat.size()<<endl;//assert(boat_num == (int)boat.size());
    for(int i = 0; i < boat_num; i ++)
    {
        int id, goods_num, x, y, dir, status;
        cin>>id>>goods_num>>y>>x>>dir>>status;
        boat[i]->set(id, goods_num, x, y, dir, status);
        boat[i]->print();
    }
    char okk[100];
    cin>>okk;
}
void action()
{
    for(int i=0;i<robot.size();++i)
        robot[i]->action();
    for(int i=0;i<boat.size();++i)
        boat[i]->action();
}
void purchase()
{
    if(boat_purchase_point[0]->can_purchase(money, boat))
    {
        Boat* boa = new BoatNorm();
        boat.push_back(boa);
        boat_purchase_point[0]->purchase(money);
    }
    if(robot_purchase_point[0]->can_purchase(money, robot))
    {
        Robot* rob = new RobotNorm();
        robot.push_back(rob);
        robot_purchase_point[0]->purchase(money);
    }
}


int main()
{
    Init();
    while(cin>>frame_id)
    {
        Input();
        action();
        purchase();

        puts("OK");
        fflush(stdout);
    }
    return 0;
}