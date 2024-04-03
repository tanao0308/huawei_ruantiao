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
        scanf("%s", grid[i]);
    }
    ProcessMap();
    scanf("%d", &berth_num);
    for(int i = 0; i < berth_num; i ++)
    {
        int id, x, y, loading_speed;
        scanf("%d%d%d%d", &id, &y, &x, &loading_speed);

        int lx=x,ly=y,rx=x,ry=y;
        while(grid[ly][lx-1]=='B')lx--;
        while(grid[ly-1][lx]=='B')ly--;
        while(grid[ry][rx+1]=='B')rx++;
        while(grid[ry+1][rx]=='B')ry++;
        Berth* ber = new Berth(id,lx,ly,rx,ry,loading_speed);
        // ber->print();
        berth.push_back(ber);
    }
    scanf("%d", &boat_capacity);
    char okk[100];
    scanf("%s", okk);
    printf("OK\n");
    fflush(stdout);
}
void Input()
{
    scanf("%d", &money);

    scanf("%d", &goods_num);
    for(int i = 0; i < goods_num; i ++)
    {
        int x, y, val;
        scanf("%d%d%d", &y, &x, &val);
        if(val == 0) val = 0;
    }

    scanf("%d", &robot_num);assert(robot_num == (int)robot.size());
    for(int i = 0; i < robot_num; i ++)
    {
        int id, goods_num, x, y;
        scanf("%d%d%d%d", &id, &goods_num, &y, &x);
        robot[i]->set(id, goods_num, x, y);
        robot[i]->print();
    }

    scanf("%d", &boat_num);assert(boat_num == (int)boat.size());
    for(int i = 0; i < boat_num; i ++)
    {
        int id, goods_num, x, y, dir, status;
        scanf("%d%d%d%d", &id, &goods_num, &y, &x, &dir, &status);
        boat[i]->set(id, goods_num, x, y, dir, status);
        boat[i]->print();
    }
    char okk[100];
    scanf("%s", okk);
}
void purchase()
{
    return;

    if(robot.size()>=3)return;
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
void action()
{
    for(int i=0;i<robot.size();++i)
        robot[i]->action();
    for(int i=0;i<boat.size();++i)
        boat[i]->action();
    purchase();
}


int main()
{
    Init();
    while(scanf("%d", &frame_id) != EOF)
    {
        Input();
        action();
        // for(int i = 0; i < robot_num; i ++)
        //     printf("move %d %d\n", i, rand() % 4);

        // for(int i = 0; i < boat_num; i ++){
        //     int status = abs(rand()) % 2;
        //     if(status == 1)
        //         printf("ship %d\n", i);
        //     else
        //         printf("rot %d %d\n", i, rand() % 2);
        // }
        puts("OK");
        fflush(stdout);
    }
    return 0;
}