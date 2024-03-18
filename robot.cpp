#include<bits/stdc++.h>
#include "global.h"
using namespace std;

extern const int n;
extern const int N;
extern const int robot_num;
extern const int boat_num;
extern const int berth_num;
extern const int dx[4],dy[4];
extern int t0,money,boat_capacity,all_gds_val;
extern char mp[200][200];
extern Gds exist_gds[200][200];
extern Berth berth[10];

class ROBOT
{
public:
    int id,t0,x,y;
    bool gds,status;Gds tar_gds;// gds:手上货物价值，status 0/1:是否能动，tar_gds:目标货物（在去拿和拿回来之间的过程）
    stack<int>op_sta;deque<int>op;queue<node>q;
    void take_action()
    {
        int pre_action=get_pre_action();
        if(pre_action==0)
            printf("get %d\n",id);
        else if(pre_action==1)
            printf("pull %d\n",id);

        int action=get_action();
        if(action>=0&&action<4)
            printf("move %d %d\n",id,action);
    }
    int target_berth;bool get_start_berth;
    int get_berth()
    {//找到离自己最近的港口
        if(!get_start_berth)
        {
            get_start_berth=1;int select=1e9;
            for(int b=0;b<10;++b)if(berth[b].dis[y][x])
            {
                if(berth[b].start_select<select)
                    select=berth[b].start_select,target_berth=b;
            }
            berth[target_berth].start_select++;
            return target_berth;
        }
        if(target_berth!=-1)return target_berth;
        for(int b=0;b<10;++b)if(berth[b].dis[y][x]<1e9&&berth[b].robots<1)//2:代表一个泊位最多2 robot
            if(target_berth==-1||berth[target_berth].dis[y][x]>berth[b].dis[y][x])
                target_berth=b;
        if(target_berth==-1)return -1;
        berth[target_berth].robots++;
        return target_berth;
    }
    bool in_berth(int id,int x,int y)
    {
        if(id==-1)return 0;
        if(x<berth[id].x||y<berth[id].y)return 0;
        if(x>=berth[id].x+4||y>=berth[id].y+4)return 0;
        return 1;
    }
    bool get_object;//当前帧是否需要拿物品
    int get_pre_action()
    {
        if(!status)return -1;
        if(!op.empty())return -1;
        int ber=get_berth();if(ber==-1)return -1;
        if(!in_berth(ber,x,y))
        {
            if(!gds&&get_object)
            {
                // cerr<<"get object"<<endl;while(1);
                gds=1;get_object=0;
                berth[ber].robots--;
                target_berth=-1;
                return 0;
            }
            else return -1;
        }
        else
        {
            if(gds)
            {
                gds=0;
                berth[ber].gds_num++;
                berth[ber].total_gds_value+=tar_gds.v;
                return 1;
            }
            else return -1;
        }
    }
    int man_dis(int x,int y,int xx,int yy) {return abs(x-xx)+abs(y-yy);}
    Gds get_gds(int ber)
    {
        int val0=0,tx=-1,ty=-1;
        for(int i=0;i<n;++i)
            for(int j=0;j<n;++j)if(exist_gds[i][j].t&&berth[ber].dis[i][j]<1e9)
            {
                int val=(double)exist_gds[i][j].v/man_dis(x,y,j,i);
                if(val>val0)val0=val,tx=j,ty=i;
            }
        if(tx==-1)return (Gds){-1,-1,-1,-1,-1};
        Gds res=exist_gds[ty][tx];
        exist_gds[ty][tx].t=0;
        all_gds_val+=exist_gds[ty][tx].v;
        return res;
    }
    void get_queue()
    {//route数组表示地图上某点按route走即可到港口
        int ber=get_berth();if(ber==-1)return;
        Gds gds=get_gds(ber);if(gds.x==-1)return;
        tar_gds=gds;

        while(!op_sta.empty())op_sta.pop();
        while(!op.empty())op.pop_front();
        while(!in_berth(ber,gds.x,gds.y))
        {
            int action=berth[ber].route[gds.y][gds.x];
            if(action==-1)return;
            op_sta.push(action^1);
            gds.x+=dx[action];
            gds.y+=dy[action];
        }
        while(gds.x!=x||gds.y!=y)
        {
            int action=rand()%4;
            Gds gds2=(Gds){gds.x+dx[action],gds.y+dy[action],-1,-1};
            if(man_dis(gds2.x,gds2.y,x,y)<man_dis(gds.x,gds.y,x,y))
            {
                op_sta.push(action^1);
                gds=gds2;
            }
        }
        while(!op_sta.empty())
        {
            op.push_back(op_sta.top());
            op_sta.pop();
        }
    }
    bool walkable(int x,int y)
    {
        if(x<0||y<0||x>=n||y>=n)return 0;
        if(mp[y][x]!='.'&&mp[y][x]!='B')return 0;
        return 1;
    }
    int pri;
    int get_priority()
    {
        if(t0%4!=1)return pri;
        int ber=get_berth();if(ber==-1)return 1e9;
        pri=10*berth[ber].dis[y][x]+id;
        return pri;
    }
    int other_robot_dis(int x,int y,int x0,int y0)//需要避让就返回其他机器人离自己的最近值，否则返回inf
    {
        int dis=1e9;int ber=get_berth();if(ber==-1)return 1e9;
        for(int i=0;i<robot_num;++i)if(i!=id&&berth[ber].dis[robot_data[i].y][robot_data[i].x]<1e9)
        {
            if(pri>robot_data[i].pri)continue;//优先级越高，越能行动
            dis=min(dis,man_dis(x,y,robot_data[i].x,robot_data[i].y));
        }
        return dis;
    }
    int check_coll()//检查当前行动机器人是否会碰撞，如果会碰撞且需要避让，那么返回应进行的避让操作
    {//一个机器人应当距离其他机器人两格及以上
        if(other_robot_dis(x,y,x,y)>=3)return -1;
        int randi[4]={0,1,2,3};random_shuffle(randi,randi+4);
        int fin_act=-1,fin_dis=0;
        for(int i=0;i<4;++i)
        {
            int act=randi[i];
            // int act=i;
            int xx=x+dx[act],yy=y+dy[act];
            int dis=other_robot_dis(xx,yy,x,y);
            if(dis>fin_dis&&walkable(xx,yy))
                fin_act=act,fin_dis=dis;
        }
        if(fin_dis>=2)return fin_act;
        // cerr<<"check_coll()"<<endl;//按理来说不应该运行到这里
        // while(1);
        return 4;
    }
    int get_action()
    {
        if(!op.empty()) {//有操作序列那么按操作序列做
            int res=op.front();
            op.pop_front();

            //以下为防撞
            int act=check_coll();
            if(act!=-1)
            {
                op.push_front(res);
                if(act==4)res=-1;
                else op.push_front(act^1),res=act;
            }

            if(op.empty())get_object=1;
            return res;
        }
        //以下是没操作序列的情况
        int ber=get_berth();if(ber==-1)return -1;
        if(in_berth(ber,x,y)) {//如果当前在港口（则当前手上必然是空的）
            get_queue();
            if(op.empty())//目前没有物体能拿，就休息一会儿
                return -1;
            else
            {
                int res=op.front();
                op.pop_front();

                //以下为防撞
                int act=check_coll();
                if(act!=-1)
                {
                    op.push_front(res);
                    if(act==4)res=-1;
                    else op.push_front(act^1),res=act;
                }

                if(op.empty())get_object=1;
                return res;
            }
        }
        else {//如果当前不在港口且没有操作队列，那就要返回港口，无论手上有没有东西
            int ber=get_berth();if(ber==-1)return -1;
            int res=berth[ber].route[y][x];
            
            //以下为防撞
            int act=check_coll();
            if(act!=-1)
            {
                res=act;
                if(act==4)res=-1;
            }

            return res;
        }
    }
};