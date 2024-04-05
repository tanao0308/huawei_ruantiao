#ifndef LANDS_CPP
#define LANDS_CPP

#include<bits/stdc++.h>
using namespace std;

extern char grid[200][200];
const int dx[4]={1,-1,0,0},dy[4]={0,0,-1,1}; //右左上下 0 1 2 3
const int bx[4]={2,-2,1,-1},by[4]={1,-1,-2,2}; //船在四个方向时以核心点为原点最远的点的相对位置
const int clockwise_dir[4]={3,2,0,1};
struct Bfs_boat
{
    int x, y, dir;
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
        return Bfs_boat{x+dx[dir],y+dy[dir],dir};
    }
    Bfs_boat rot(int i)
    {
        int nx,ny,ndir=dir;
        if(i==0)
        {
            nx=x+2*dx[dir];
            ny=y;
            ndir=clockwise_dir[(dir+1)%4];
        }
        else
        {
            nx=x+bx[dir]/abs(bx[dir]);
            ny=y+by[dir]/abs(by[dir]);
            ndir=clockwise_dir[(dir-1+4)%4];
        }
        return Bfs_boat{nx,ny,ndir};
    }
    Bfs_boat back_ship()
    {
        return Bfs_boat{x-dx[dir],y-dy[dir],dir};
    }
    Bfs_boat back_rot(int i)
    {
        int nx,ny,ndir=dir;
        if(i==0)
        {
            ndir=clockwise_dir[(dir-1+4)%4];
            nx=x-2*dx[ndir];
            ny=y;
        }
        else
        {
            ndir=clockwise_dir[(dir+1)%4];
            nx=x-bx[dir]/abs(bx[dir]);
            ny=y-by[dir]/abs(by[dir]);
        }
        return Bfs_boat{nx,ny,ndir};
    }
};

class Land {
protected:
    queue<Bfs_boat>q_boat;
public:
    int boat_map[200][200][4]; //-2表示到达终点，-1表示此状态非法，0-2表示当前状态应该走这个走法
    int robot_map[200][200];
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
                    // cerr<<boat_map[i][j][dir]<<" ";
                }
                cerr<<endl;
            }
        }
    }
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
                if(!v.can_put()||boat_map[v.y][v.x][v.dir]!=-1)
                    continue;
                boat_map[v.y][v.x][v.dir]=i;
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
    void init()
    {
        get_boat_map();
        get_robot_map();
    }
    virtual void init_get_boat_map() override
    {
        memset(boat_map,-1,sizeof boat_map);
        Bfs_boat u,v;
        for(int x=lx;x<=rx;++x)
            for(int y=ly;y<=ry;++y)
                for(int dir=0;dir<4;++dir)
                {
                    u=(Bfs_boat){x,y,dir};
                    if(u.can_put())
                    {
                        boat_map[u.y][u.x][u.dir]=-2;
                        q_boat.push(u);
                    }
                }
    }
    void get_robot_map()
    {
        return;
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
    void init()
    {
        get_boat_map();
    }
    virtual void init_get_boat_map() override
    {
        memset(boat_map,-1,sizeof boat_map);
        Bfs_boat u,v;
        for(int dir=0;dir<4;++dir)
        {
            u=(Bfs_boat){x,y,dir};
            if(u.can_put())
            {
                boat_map[u.y][u.x][u.dir]=-2;
                q_boat.push(u);
            }
        }
    }
};

#endif