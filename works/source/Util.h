#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED
#include "PokerUtil.h"


#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;


static string value_str[] = {
	"",
	"Straight Flush",
	"Four of a Kind",
	"Full House",
	"Flush",
	"Straight",
	"Three of a Kind",
	"Two Pair",
	"One Pair",
	"High Card"
};

enum SuitOfPoker
{spade, heart, club, diamond};
enum RankOfPoker
{deuce,trey,four,five,six,seven,eight,nine,ten,jack,queen,king,ace};
int prim[] = {2,3,5,7,11,13,17,19,23,29,31,37,41};
/*
+--------+--------+--------+--------+
|xxxbbbbb|bbbbbbbb|cdhsrrrr|xxpppppp|
+--------+--------+--------+--------+

p = prime number of rank (deuce=2,trey=3,four=5,...,ace=41)
r = rank of card (deuce=0,trey=1,four=2,five=3,...,ace=12)
cdhs = suit of card (bit turned on based on suit of card)
b = bit turned on depending on rank of card
*/
class Poker
{
public:

    int data;
    Poker(int no)
    {
        int num = no%13;
        int suit = no/13;
        data = 0;
        data = data | ((1<<((int)num))<<16);
        data = data | ((1<<((int)suit))<<12);
        data = data | (((int)num)<<8);
        data = data | prim[(int)num];
    }
    Poker(RankOfPoker num,SuitOfPoker suit)
    {
        data = 0;
        data = data | ((1<<((int)num))<<16);
        data = data | ((1<<((int)suit))<<12);
        data = data | (((int)num)<<8);
        data = data | prim[(int)num];
    }

    pair<RankOfPoker,SuitOfPoker> GetInfo() const
    {
        int s  = (data&0xF000) >> 12;
        for (int i = 0; i<4; ++i)
        {
            if (s & (1<<i))
            {
                s = i;
                break;
            }
        }
        int r = (data&0x0F00) >> 8;
        return make_pair((RankOfPoker)r,(SuitOfPoker)s);
    }
    string getstring()
    {
        pair<RankOfPoker,SuitOfPoker> p = GetInfo();
        string tem;
        stringstream s(tem);
        s<<((int)p.first)+1<<"/"<<p.second;
        getline(s,tem);
        return tem;
    }
    bool operator<(const Poker & t) const
    {
        return GetInfo().first < t.GetInfo().first;
    }
    int GetNum()
    {
        pair<RankOfPoker,SuitOfPoker> p = GetInfo();
        return p.first+p.second*13;
    }
};

class HandCards
{
public:
    vector<Poker> data;
    void add(Poker & k)
    {
         data.push_back(k);
    }
    int GetDistinct()
    {
        if (data.size()!=5)
            return -1;
        int tem[5];
        for (int i=0;i<5;++i)
            tem[i] = data[i].data;
        return   eval_5hand(tem);
    }
    int GetUnique()
    {
         if (data.size()!=5)
            return -1;
         int i = GetDistinct()-1;
         return Punique[i];
    }
    string GetClass()
    {
        int i = GetDistinct();
        return value_str[hand_rank(i)];
    }
};

class Init
{
public:

    Init()
    {
        cout<<"System is initiated!"<<endl;
        SystemInit();
    }
}hgsdfjhgyu;

#endif // UTIL_H_INCLUDED
