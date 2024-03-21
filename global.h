#ifndef GLOBAL_H
#define GLOBAL_H

#include <bits/stdc++.h>
using namespace std;

extern const int n;
extern const int robot_num;
extern const int dx[4],dy[4];
extern char mp[200][200];

struct node {
    int x,y,s;
    bool walkable();
};
struct Gds {
    int x,y,t,v,d;
};
extern Gds exist_gds[200][200];

struct Berth {
    int x,y;
    int transport_time;
    int loading_speed;
    int gds_num,robots;
    int start_select;
    int total_gds_value;
    int route[200][200]; //从网格上任何一点到达当前泊位的一条最短路径
    int dis[200][200]; //从网格上任何一点到达当前泊位的最短距离
    Berth(){}
    void get_route();
};
extern Berth berth[10];

struct RobotData {
    int x,y,pri;
};
extern RobotData robot_data[10];

#endif