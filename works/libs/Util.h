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


static string value_str[] =
{
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
{spade, heart,  diamond,club};
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
/*
     一张牌的类Poker
     两种构造函数
     Poker a(ace,spade);
      Poker a(12);
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

struct _result
{
    union
    {
        struct
        {
            int u,d,c;
        };
        int val[3];
    } _data;
    _result(int a,int b,int c)
    {
        _data.u=a;
        _data.d=b;
        _data.c=c;
    }
};

class Result
{
public:
    vector<_result> data;
    vector<double> pc;
    double E;
    void add(int a,int b,int c)
    {
        data.push_back(_result(a,b,c));
    }
    void Calc()
    {
        E=0;
        pc.resize(9);
        vector<int> pd(Equivalence);
        for (int i=0; i<(int)data.size(); ++i)
        {
            pc[data[i]._data.c-1]+=1;
            ++pd[data[i]._data.d-1];
        }

        for (int i=0; i<Equivalence; ++i)
        {
            E +=  pd[i]*(double)(i+1);
        }
        E /= (double)data.size();
        E /= (double)Equivalence;
        E = 1.0 - E;
        for (int i=0; i<9; ++i)
            pc[i] /= (double)data.size();
    }
    void show()
    {
        //printf("============================\n");
        for (int i=0; i<9; ++i)
        {
            //printf("%d.",i+1);
            //cout<<value_str[i+1]<<"      \t";
            //printf("%f%%\n",pc[i]);
            printf ("%f ",pc[i]);
        }
        //printf("\nE:%f%%\n",E);
        printf("%f\n",E);
        //printf("============================\n");
    }
};

 vector<Poker>  ChooseMax( vector<Poker> &data);

/*
   手牌类 HandCards
   成员函数：
       add(Poker) 添加扑克牌
       GetClass() 获得类别名
       GetUnique() 归类前排名
       GetDistinct() 归类后排名
       GetClassRank()  类的编号

*/
class HandCards
{
public:
    struct HandCardsIter
    {
        int N;
        vector<Poker> data;
        vector<int> no;
        HandCardsIter (vector<Poker> &t,int n)
        {
            N=n;
            data =t;
            for (int i=0; i<n; ++i)
            {
                no.push_back(i);
                data.push_back(Poker(i));
            }

            while (!Check())
            {
                if (!_next()) break;
            }

        }
        bool Check()
        {
            vector<bool> ifv(52);
            for (int i=0; i<(int)data.size(); ++i)
            {
                if (ifv[data[i].GetNum()]) return false;
                ifv[data[i].GetNum()] = true;
            }
            return true;
        }
        int needmodify()
        {
            if (no[0]>=52) return -1;
            bool flag = false;
            for (int i=1; i<(int)no.size(); ++i)
            {
                if (no[i]<=no[i-1])
                {
                    flag = true;
                    no[i] = no[i-1]+1;
                }
                if (no[i]>=52)
                {
                    flag=true;
                    ++no[i-1];
                    no[i]=0;
                }
            }
            return (int)flag;
        }
        bool _next()
        {
            ++no[no.size()-1];
            while (true)
            {
                int res = needmodify();
                if (res==0) break;
                if (res == -1) return false;
            }
            for (int i=data.size()-N; i<(int)data.size(); ++i)
                data[i]= Poker(no[i-data.size()+N]);
            return true;
        }
        bool next()
        {
            if (!_next())
                return false;
            while (!Check())
            {
                if (!_next())
                    return false;
            }
            return true;
        }
    };
    vector<Poker> data;
    HandCards(){}

    HandCards(vector<Poker> &t)
    {
        data =t;
    }
    void add(const Poker & k)
    {
        data.push_back(k);
    }
    int GetDistinct()
    {
        if (data.size()!=5)
            return -1;
        int tem[5];
        for (int i=0; i<5; ++i)
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
    int GetClassRank()
    {
        int i = GetDistinct();
        return hand_rank(i);
    }
    void print()
    {
        int n = data.size();
        int * hand = new int[n];
        for (int i=0; i<n; ++i)
            hand[i]=data[i].data;
        print_hand(hand,n);
        printf("\n");
        delete []hand;
    }

    Result Analyze(int k = 5 )
    {
        Result  res;
        if (k == (int)data.size())
            return res;
        int nn=0;
        if (k>(int)data.size())
        {
            int N = k - (int)data.size();
            HandCardsIter iter(data,N);

            while (iter.Check())
            {
                ++nn;
                HandCards te = iter.data;
                vector<Poker> tt = (ChooseMax(te.data));
                HandCards tem(tt);
                int a = tem.GetUnique();
                int b = tem.GetDistinct();
                int c = tem.GetClassRank();
                res.add(a,b,c);
                //if (nn%10000==0) printf("%d\n",nn);
                if (!iter.next()) break;
            }
        }
        //printf("%d\n",nn);
        return res;
    }

};

int mmax;
HandCards mt;
vector<int> Index;
vector<bool> cifv;
void choosedfs(vector<Poker>  &data,int i,int cnt,int tag)
{
    if (cnt==tag)
    {
        HandCards tem;
        tem.add(data[0]);
        tem.add(data[1]);

        for (int j=0; j<(int)Index.size(); ++j)
        {
            if (!cifv[j]) continue;
            tem.add(data[Index[j]]);
        }

        int pp = tem.GetDistinct();
        if (pp<mmax)
        {

            mmax = pp;
            mt = tem;
        }
        return;
    }
    if (i>=(int)Index.size()) return;

    cifv[i]=true;
    choosedfs(data,i+1,cnt+1,tag);
    cifv[i]=false;
    choosedfs(data,i+1,cnt,tag);
}
 vector<Poker>  ChooseMax( vector<Poker> &data)
{
    Index.clear();
    cifv.clear();
    for (int i=2; i<(int)data.size(); ++i)
    {
        Index.push_back(i);
        cifv.push_back(false);
    }
    mmax =10000;
    choosedfs(data,0,0,3);
    return mt.data;
}


class Init
{
public:

    Init()
    {
        cout<<"System is initiated!"<<endl;
        SystemInit();
    }
} hgsdfjhgyu;

#endif // UTIL_H_INCLUDED
