#include <bits/stdc++.h>
#include "global.h"

bool node::walkable()
{
    if(x<0||y<0||x>=n||y>=n)return 0;
    if(mp[y][x]!='.'&&mp[y][x]!='B')return 0;
    return 1;
}
void Berth::get_route()
{
    queue<node>q;
    memset(dis,0x3f,sizeof dis);
    memset(route,-1,sizeof route);
    for(int i=0;i<4;++i)
        for(int j=0;j<4;++j)
        {
            dis[y+i][x+j]=0;
            route[y+i][x+j]=4;
            q.push((node){x+j,y+i,0});
        }
    while(!q.empty())
    {
        node u=q.front();q.pop();
        int randi[4]={0,1,2,3};random_shuffle(randi,randi+4);
        for(int ii=0;ii<4;++ii)
        {
            int i=randi[ii];
            node v=(node){u.x+dx[i],u.y+dy[i],u.s+1};
            if(!v.walkable()||route[v.y][v.x]!=-1)continue;
            dis[v.y][v.x]=v.s;
            route[v.y][v.x]=i^1;
            q.push(v);
        }
    }
}