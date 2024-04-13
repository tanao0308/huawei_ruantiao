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
vector<RobotPurchasePoint*> robot_purchase_point;
vector<BoatPurchasePoint*> boat_purchase_point;
vector<DeliveryPoint*> delivery_point;

int goods_num = 0;
int frame_id = 0;
int money, boat_capacity;
int Boat::boat_capacity = 0;

char grid[N][N];
Goods goods_map[200][200];

int robot_create_money;

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
            {
                DeliveryPoint* del = new DeliveryPoint(j,i);
                delivery_point.push_back(del);
            }
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
    {
        berth[i]->init();
        // berth[i]->print_boat_map();
        // while(1);
    }
    for(int i=0;i<delivery_point.size();++i)
    {
        delivery_point[i]->init();
        // delivery_point[i]->print_boat_map();
        // while(1);
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
    // cerr<<robot_num<<"robot"<<robot.size()<<endl;//assert(robot_num == (int)robot.size());
    for(int i = 0; i < robot_num; i ++)
    {
        int id, goods_num, x, y;
        cin>>id>>goods_num>>y>>x;
        robot[i]->set(id, goods_num, x, y);
        // robot[i]->print();
    }

    cin>>boat_num;
    // cerr<<boat_num<<"boat"<<boat.size()<<endl;//assert(boat_num == (int)boat.size());
    for(int i = 0; i < boat_num; i ++)
    {
        int id, goods_num, x, y, dir, status;
        cin>>id>>goods_num>>y>>x>>dir>>status;
        boat[i]->set(id, goods_num, x, y, dir, status);
        // boat[i]->print();
    }

    cerr<<"robot_create_money "<<robot_create_money<<endl;

    char okk[100];
    cin>>okk;
}
void action()
{
    cerr<<"action aaa"<<endl;
    for(int i=0;i<robot.size();++i)
        robot[i]->action();
    cerr<<"action bbb"<<endl;
    for(int i=0;i<boat.size();++i)
        boat[i]->action();
    cerr<<"action ccc"<<endl;
}
void purchase()
{
    int bpp=0,rpp=0,ber=0,dp=0;
    bpp = rand()%boat_purchase_point.size();
    ber = rand()%berth.size();
    dp = rand()%delivery_point.size();
    if(boat_purchase_point[bpp]->can_purchase(money, boat))
    {
        BoatNorm* boa = new BoatNorm();
        boa->set_berth(berth[ber]);
        boa->set_delivery_point(delivery_point[dp]);
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
        cerr<<"aaa"<<endl;
        action();
        cerr<<"bbb"<<endl;
        purchase();
        cerr<<"ccc"<<endl;

        puts("OK");
        fflush(stdout);
    }
    return 0;
}