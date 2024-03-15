#include<bits/stdc++.h>
#include "global.h"
using namespace std;

extern const int n;
extern const int N;
extern const int robot_num;
extern const int boat_num;
extern const int berth_num;
extern const int dx[4],dy[4];;
extern int id,money,boat_capacity;
extern char mp[200][200];
extern int mp_gds[200][200];
extern int route[200][200];
extern Berth berth[10];

class ROBOT
{
public:
    int id,x,y;
    bool gds,status;// gds 0/1:手上有无货物，status 0/1:是否能动
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
    int get_pre_action()
    {
        if(!status)return -1;
        if(mp[y][x]!='B')
        {
            if(mp_gds[y][x]&&!gds)
            {
                mp_gds[y][x]=0;
                gds=1;
                return 0;
            }
            else
                return -1;
        }
        else
        {
            if(gds)
            {
                gds=0;
                return 1;
            }
            else
                return -1;
        }
    }

    bool walkable(int x,int y)
    {
        if(x<0||y<0||x>=n||y>=n)return 0;
        if(mp[y][x]!='.'&&mp[y][x]!='B')return 0;
        return 1;
    }
    int man_dis(int x,int y,int xx,int yy)
    {
        return abs(x-xx)+abs(y-yy);
    }
    stack<int>op_sta;queue<int>op;queue<node>q;
    // int pre[1][1],r=0;//9=2*r+1
    void get_queue()
    {
        int ber=mp_ber[y][x];
        if(ber==-1)return;
        Gds gds=berth[ber].get_gds();
        if(gds.x==-1)return;
        while(!op_sta.empty())op_sta.pop();
        while(!op.empty())op.pop();
        while(mp[gds.y][gds.x]!='B')
        {
            int action=berth[ber].route[gds.y][gds.x];
            op_sta.push(action);
            gds.x-=dx[action];
            gds.y-=dy[action];
        }
        while(gds.x!=x||gds.y!=y)
            for(int action=0;action<4;++action)
            {
                Gds gds2=(Gds){gds.x-dx[action],gds.y-dy[action],-1,-1};
                if(man_dis(gds2.x,gds2.y,x,y)>man_dis(gds.x,gds.y,x,y))
                {
                    op_sta.push(action);
                    gds=gds2;
                }
            }
        while(!op_sta.empty())
        {
            op.push(op_sta.top());
            op_sta.pop();
        }

        /*
        while(!q.empty())q.pop();
        q.push((node){x,y,0});
        memset(pre,-1,sizeof pre);
        node tar={-1,-1,-1};
        while(!q.empty())
        {
            node u=q.front();q.pop();
            if(mp_gds[u.y][u.x]){tar=u;break;}
            for(int i=0;i<4;++i)
            {
                node v=(node){u.x+dx[i],u.y+dy[i],u.s+1};
                if(v.s>r||!walkable(v.x,v.y)||pre[v.y-(y-r)][v.x-(x-r)]!=-1)
                    continue;
                q.push(v);
                pre[v.y-(y-r)][v.x-(x-r)]=i;
            }
        }
        if(tar.s==-1)return;
        stack<int>op_sta;
        while(tar.x!=x||tar.y!=y)
        {
            int action=pre[tar.y-(y-r)][tar.x-(x-r)];
            op_sta.push(action);
            tar.x-=dx[action];
            tar.y-=dy[action];
        }
        while(!op_sta.empty())
        {
            op.push(op_sta.top());
            op_sta.pop();
        }
        */
    }
    int rand_walk()
    {
        for(int i=0;i<10;++i)
        {
            int j=rand()%4;
            if(walkable(x+dx[j],y+dy[j]))
                return j;
        }
        return -1;
    }
    int get_action()
    {
        if(!gds)
        {
            if(op.empty()&&mp[y][x]=='B')
                get_queue();

            if(op.empty())
                return rand_walk();
            else
            {
                int res=op.front();
                op.pop();
                return res;
            }
        }
        else
        {
            return berth[mp_ber[y][x]].route[y][x];
            // return route[y][x];
        }
    }
};