#ifndef BERTH_CPP
#define BERTH_CPP

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
};

class Berth {
private:
    int id, lx, ly, rx, ry;
    int loading_speed;
public:
    int boat_map[200][200][4];
    int robot_map[200][200];
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
        init_boat_map();
        init_robot_map();
    }
    bool vis[200][200][4];
    void init_boat_map()
    {
        queue<Bfs_boat>q;
        Bfs_boat u,v;
        for(int x=lx;x<=rx;++x)
            for(int y=ly;y<=ry;++y)
                for(int dir=0;dir<4;++dir)
                {
                    u=(Bfs_boat){x,y,dir};
                    if(u.can_put())
                        q.push(u);
                }
        while(!q.empty())
        {
            u=q.front();
            vis[u.y][u.x][u.dir]=1;
            for(int i=0;i<3;++i)
            {
                if(i==2)
                    v=u.ship();
                else
                    v=u.rot(i);
                if(vis[v.y][v.x][v.dir]||!can_put_boat(v.x,v.y,v.dir))
                    continue;
                q.push(v);
            }
        }
    }
    bool can_put_boat(int x,int y,int dir)
    {

    }
    void init_robot_map()
    {
        return;
    }
};

#endif