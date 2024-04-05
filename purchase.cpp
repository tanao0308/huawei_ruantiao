#ifndef PURCHASE_CPP
#define PURCHASE_CPP

#include<bits/stdc++.h>
#include "boat.cpp"
#include "robot.cpp"
#include "lands.cpp"
using namespace std;

class PurchasePoint {
protected:
    int x, y;

public:
    virtual ~PurchasePoint(){};

    PurchasePoint(int x, int y)
    {
        this -> x = x;
        this -> y = y;
    }
    virtual void print() = 0;
    virtual void purchase(int&money) = 0;
};

class BoatPurchasePoint : public PurchasePoint {
private:
    static const int boat_price=8000;
public:
    BoatPurchasePoint(int x, int y) : PurchasePoint(x, y) {}
    ~BoatPurchasePoint() {}

    void print() override 
    {
        cerr<<"BoatPurchasePoint: x="<<x<<", y="<<y<<endl;
    }
    bool can_purchase(int money, vector<Boat*>boat) //判断当前售船点是否能买船
    {
        if(boat.size()>=1)return 0;
        if(money<boat_price)return 0;
        for(int i=-1;i<=2;++i)
            for(int j=-1;j<=3;++j)
                for(int k=0;k<boat.size();++k)
                    if(boat[k]->intersect(x+j,y+i))
                        return 0;
        return 1;
    }
    void purchase(int&money) override 
    {
        money -= boat_price;
        printf("lboat %d %d\n", y, x);
    }
};

class RobotPurchasePoint : public PurchasePoint {
private:
    static const int robot_price=2000;
public:
    RobotPurchasePoint(int x, int y) : PurchasePoint(x, y) {}
    ~RobotPurchasePoint() {}

    void print() override 
    {
        cerr<<"RobotPurchasePoint: x="<<x<<", y="<<y<<endl;
    }
    bool can_purchase(int money, vector<Robot*>robot)
    {
        if(robot.size()>=10)return 0;
        if(money<robot_price)return 0;
        for(int i=0;i<robot.size();++i)
            if(robot[i]->intersect(x,y))
                return 0;
        return 1;
    }
    void purchase(int&money) override 
    {
        money -= robot_price;
        printf("lbot %d %d\n", y, x);
    }
};

#endif