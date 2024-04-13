#include <bits/stdc++.h>
#include "boat.cpp"
#include "robot.cpp"
#include "lands.cpp"
#include "purchase.cpp"
using namespace std;
const int N = 200;

int robot_num = 0;
int boat_num = 0;
int berth_num = 0;
vector<BoatNorm*>boat;
vector<RobotNorm*>robot;
vector<Berth*>berth;
vector<DeliveryPoint*> delivery_point;
vector<RobotPurchasePoint*> robot_purchase_point;
vector<BoatPurchasePoint*> boat_purchase_point;

int goods_num = 0;
int frame_id = 0;
int money, boat_capacity;
int Boat::boat_capacity = 0;

char grid[N][N];
Goods goods_map[200][200];

unsigned long long map_hash;
int map_id;
int robot_create_money;

void ProcessMap()
{
    map_hash=0;
    for(int i = 0; i < N; i ++){
        for(int j = 0; j < N; j ++){
            if(grid[i][j] == 'R')
            {
                RobotPurchasePoint* rpp = new RobotPurchasePoint(j,i);
                robot_purchase_point.push_back(rpp);
            }
            else if(grid[i][j] == 'S')
            {
                BoatPurchasePoint* bpp = new BoatPurchasePoint(j,i);
                boat_purchase_point.push_back(bpp);
            }
            else if(grid[i][j] == 'T')
            {
                DeliveryPoint* del = new DeliveryPoint(j,i);
                delivery_point.push_back(del);
            }
            map_hash=map_hash*137+grid[i][j];
        }
    }
    cerr<<map_hash<<endl;
    if(map_hash==(unsigned long long)16656903604756406537)
        map_id=4;
    else if(map_hash==(unsigned long long)16476000125684867378)
        map_id=5;
    else 
        map_id=6;
    cerr<<map_id<<endl;
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
    {
        berth[i]->init();
    }
    for(int i=0;i<delivery_point.size();++i)
    {
        delivery_point[i]->init();
    }

    char okk[100];
    cin>>okk;
    cout<<"OK\n";
    fflush(stdout);
}
void Input()
{
    cin>>money;

    for(int i=0;i<200;++i)  
        for(int j=0;j<200;++j)
            goods_map[i][j].update();
    cin>>goods_num;
    for(int i = 0; i < goods_num; i ++)
    {
        int x, y, val;
        cin>>y>>x>>val;
        goods_map[y][x] = Goods{x,y,val,frame_id};
    }

    cin>>robot_num;
    for(int i = 0; i < robot_num; i ++)
    {
        int id, goods_num, x, y;
        cin>>id>>goods_num>>y>>x;
        robot[i]->set(id, goods_num, x, y);
    }

    cin>>boat_num;
    for(int i = 0; i < boat_num; i ++)
    {
        int id, goods_num, x, y, dir, status;
        cin>>id>>goods_num>>y>>x>>dir>>status;
        boat[i]->set(id, goods_num, x, y, dir, status);
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
    int bpp=0,rpp=0,ber=0,dp=0;
    bpp = rand()%boat_purchase_point.size();
    if(boat_purchase_point[bpp]->can_purchase(money, boat))
    {
        BoatNorm* boa = new BoatNorm();
        boa->set_cruise(berth,delivery_point[dp]);
        boat.push_back(boa);
        boat_purchase_point[bpp]->purchase(money);
    }
    rpp = rand()%robot_purchase_point.size();
    ber = rand()%berth.size();
    if(robot_purchase_point[rpp]->can_purchase(money, robot))
    {
        RobotNorm* rob = new RobotNorm();
        rob->set_berth(berth[ber]);
        rob->robot = &robot;
        robot.push_back(rob);
        robot_purchase_point[rpp]->purchase(money);
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