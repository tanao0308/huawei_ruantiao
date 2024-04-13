#ifndef LANDS_CPP
#define LANDS_CPP

#include<bits/stdc++.h>
using namespace std;

extern int frame_id;
extern char grid[200][200];
const int dx[4]={1,-1,0,0},dy[4]={0,0,-1,1}; //右左上下 0 1 2 3
const int bx[4]={2,-2,1,-1},by[4]={1,-1,-2,2}; //船在四个方向时以核心点为原点最远的点的相对位置
const int cow_dir[4]={3,2,0,1},ccw_dir[4]={2,3,1,0};

struct Goods {
    int x,y,v,t;
    void update()
    {
        if(frame_id>=t+1000||v==0)
            x=-1,y=-1,v=-1,t=-1;
    }
};
extern Goods goods_map[200][200];
struct Bfs_boat
{
    int x, y, dir;
    int step;
    bool boat_available(char c)
    {
        if(c=='*'||c=='~'||c=='S'||c=='B'||c=='K'||c=='C'||c=='c'||c=='T')
            return 1;
        return 0;
    }
    bool can_put()
    {
        int lx=x,ly=y;
        int rx=x+bx[dir],ry=y+by[dir];
        if(lx>rx)swap(lx,rx);
        if(ly>ry)swap(ly,ry);
        if(lx<0||ly<0||rx>=200||ry>=200)return 0;
        for(int i=ly;i<=ry;++i)
            for(int j=lx;j<=rx;++j)
                if(!boat_available(grid[i][j]))
                    return 0;
        return 1;
    }
    Bfs_boat ship()
    {
        return Bfs_boat{x+dx[dir],y+dy[dir],dir,step};
    }
    Bfs_boat rot(int i)
    {
        int nx,ny,ndir=dir;
        if(i==0)
        {
            int delta_x[4]={2,-2,0,0},delta_y[4]={0,0,-2,2};
            nx=x+delta_x[dir];
            ny=y+delta_y[dir];
            ndir=cow_dir[dir];
        }
        else
        {
            int delta_x[4]={1,-1,1,-1},delta_y[4]={1,-1,-1,1};
            nx=x+delta_x[dir];
            ny=y+delta_y[dir];
            ndir=ccw_dir[dir];
        }
        return Bfs_boat{nx,ny,ndir,step};
    }
    Bfs_boat back_ship()
    {
        return Bfs_boat{x-dx[dir],y-dy[dir],dir,step};
    }
    Bfs_boat back_rot(int i)
    {
        int nx,ny,ndir;
        if(i==0)
        {
            int delta_x[4]={2,-2,0,0},delta_y[4]={0,0,-2,2};
            ndir=ccw_dir[dir];
            nx=x-delta_x[ndir];
            ny=y-delta_y[ndir];
        }
        else
        {
            int delta_x[4]={1,-1,1,-1},delta_y[4]={1,-1,-1,1};
            ndir=cow_dir[dir];
            nx=x-delta_x[ndir];
            ny=y-delta_y[ndir];
        }
        return Bfs_boat{nx,ny,ndir,step};
    }
};
struct Bfs_robot
{
    int x, y;
    int step;
    bool robot_available(char c)
    {
        if(c=='.'||c=='>'||c=='R'||c=='B'||c=='C'||c=='c')
            return 1;
        return 0;
    }
    bool can_put()
    {
        if(x<0||y<0||x>=200||y>=200)return 0;
        if(!robot_available(grid[y][x]))
            return 0;
        return 1;
    }
    Bfs_robot move(int i)
    {
        return Bfs_robot{x+dx[i],y+dy[i],step};
    }
    Bfs_robot back_move(int i)
    {
        return Bfs_robot{x-dx[i],y-dy[i],step};
    }
};

class Land {
protected:
    queue<Bfs_boat>q_boat;
    queue<Bfs_robot>q_robot;
public:
    int boat_map[200][200][4]; //-2表示到达终点，-1表示此状态非法，0-2表示当前状态应该走这个走法
    int boat_dis[200][200][4];
    virtual ~Land(){};
    void print_boat_map()
    {
        for(int dir=0;dir<4;++dir)
        {
            cout<<"---------------"<<dir<<"---------------"<<endl;
            for(int i=0;i<200;++i)
            {
                for(int j=0;j<200;++j)
                {
                    if(boat_map[i][j][dir]==-2)cerr<<"# ";
                    else if(boat_map[i][j][dir]==-1)cerr<<"  ";
                    else cerr<<". ";
                }
                cerr<<endl;
            }
        }
    }
    virtual void init() = 0;
    virtual void init_get_boat_map() = 0;
    void get_boat_map()
    {
        init_get_boat_map();
        Bfs_boat u,v;
        while(!q_boat.empty())
        {
            u=q_boat.front();q_boat.pop();
            for(int i=0;i<3;++i)
            {
                if(i==2)
                    v=u.back_ship();
                else
                    v=u.back_rot(i);
                v.step++;
                if(!v.can_put()||boat_map[v.y][v.x][v.dir]!=-1)
                    continue;
                boat_map[v.y][v.x][v.dir]=i;
                boat_dis[v.y][v.x][v.dir]=v.step;
                q_boat.push(v);
            }
        }
    }
};

class Berth : public Land {
private:
    int id, lx, ly, rx, ry;
    int loading_speed;
public:
    int robot_map[200][200];
    int robot_dis[200][200];
    Berth(){}
    Berth(int id, int lx, int ly, int rx, int ry, int loading_speed) {
        this -> id = id;
        this -> lx = lx;
        this -> ly = ly;
        this -> rx = rx;
        this -> ry = ry;
        this -> loading_speed = loading_speed;
    }
    void print()
    {
        cerr<<"Berth: id="<<id<<", lx="<<lx<<", ly="<<ly<<", rx="<<rx<<", ry="<<ry<<", loading_speed="<<loading_speed<<endl;
    }
    bool in_berth(int x,int y)
    {
        if(lx<=x&&x<=rx&&ly<=y&&y<=ry)
            return 1;
        return 0;
    }
    void init() override
    {
        get_boat_map();
        get_robot_map();
    }
    virtual void init_get_boat_map() override
    {
        memset(boat_map,-1,sizeof boat_map);
        memset(boat_dis,0x3f,sizeof boat_dis);
        Bfs_boat u;
        for(int x=lx;x<=rx;++x)
            for(int y=ly;y<=ry;++y)
                for(int dir=0;dir<4;++dir)
                {
                    u=(Bfs_boat){x,y,dir,0};
                    if(u.can_put())
                    {
                        boat_map[u.y][u.x][u.dir]=-2;
                        q_boat.push(u);
                    }
                }
    }
    void init_get_robot_map()
    {
        memset(robot_map,-1,sizeof robot_map);
        memset(robot_dis,0x3f,sizeof robot_dis);
        Bfs_robot u;
        for(int x=lx;x<=rx;++x)
            for(int y=ly;y<=ry;++y)
            {
                u=(Bfs_robot){x,y,0};
                if(u.can_put())
                {
                    robot_map[u.y][u.x]=-2;
                    q_robot.push(u);
                }
            }
    }
    void get_robot_map()
    {
        init_get_robot_map();
        Bfs_robot u,v;
        while(!q_robot.empty())
        {
            u=q_robot.front();q_robot.pop();
            for(int i=0;i<4;++i)
            {
                v=u.back_move(i);
                v.step++;
                if(!v.can_put()||robot_map[v.y][v.x]!=-1)
                    continue;
                robot_map[v.y][v.x]=i;
                robot_dis[v.y][v.x]=v.step;
                q_robot.push(v);
            }
        }
    }
    Goods get_goods()
    {
        Goods gds={-1,-1,-1,-1};
        for(int i=0;i<200;++i)
            for(int j=0;j<200;++j)if(goods_map[i][j].x!=-1&&goods_map[i][j].v!=0&&robot_dis[i][j]<1e9)
            {
                Goods g = goods_map[i][j];
                if((gds.x==-1&&g.x!=-1) || (frame_id-g.t+robot_dis[i][j]<1000&&(double)g.v/robot_dis[i][j]>(double)gds.v/robot_dis[gds.y][gds.x]))
                    gds = goods_map[i][j];
            }
        goods_map[gds.y][gds.x]={-1,-1,-1,-1};
        return gds;
    }
};

class DeliveryPoint : public Land {
private:
    int x,y;
public:
    DeliveryPoint(){}
    DeliveryPoint(int x, int y) {
        this -> x = x;
        this -> y = y;
    }
    void init() override
    {
        get_boat_map();
    }
    virtual void init_get_boat_map() override
    {
        memset(boat_map,-1,sizeof boat_map);
        Bfs_boat u,v;
        for(int dir=0;dir<4;++dir)
        {
            u=(Bfs_boat){x,y,dir,0};
            if(u.can_put())
            {
                boat_map[u.y][u.x][u.dir]=-2;
                q_boat.push(u);
            }
        }
    }
};

#endif