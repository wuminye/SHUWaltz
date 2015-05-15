#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED
#include "PokerUtil.h"

#include <ctime>
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
{SPADES,HEARTS,DIAMONDS,CLUBS};
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
    int init_num;

    Poker(int no)
    {
        init_num = no;
        int num = no%13;
        int suit = no/13;
        data = 0;
        data = data | ((1<<((int)num))<<16);
        data = data | ((1<<((int)suit))<<12);
        data = data | (((int)num)<<8);
        data = data | prim[(int)num];
    }

    Poker(SuitOfPoker suit, RankOfPoker num)
    {
        data = 0;
        data = data | ((1<<((int)num))<<16);
        data = data | ((1<<((int)suit))<<12);
        data = data | (((int)num)<<8);
        data = data | prim[(int)num];
        init_num = GetNum();
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

    int GetInitNum()
    {
        return init_num;
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
        printf("============================\n");
        for (int i=0; i<9; ++i)
        {
            printf("%d.",i+1);
            cout<<value_str[i+1]<<"      \t";
            printf("%f%%\n",pc[i]);
            //printf ("%f ",pc[i]);
        }
        printf("\nE:%f%%\n",E);
     //   printf("%f\n",E);
        printf("============================\n");
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

    void clear()
    {
        data.clear();
    }

    vector<Poker> GetData()
    {
        return data;
    }

    int GetDistinct() const
    {
        if (data.size()!=5)
            return -1;
        int tem[5];
        for (int i=0; i<5; ++i)
            tem[i] = data[i].data;
        return   eval_5hand(tem);
    }

    void ShowDistinctValue() const
    {
        printf("%d",GetDistinct());
    }

    int GetUnique() const
    {
        if (data.size()!=5)
            return -1;
        int i = GetDistinct()-1;
        return Punique[i];
    }

    string GetClass() const
    {
        int i = GetDistinct();
        return value_str[hand_rank(i)];
    }

    int GetClassRank() const
    {
        int i = GetDistinct();
        return hand_rank(i);
    }

    // remove the known cards and form a new 5-hand-cards using the rest.
    void Shuffle(int num, vector<Poker> &known_pokers)
    {
        int i,k,count;
        bool flag = true;
        for (count = 0; count < num; )
        {
            flag=true;
            i = (int)(rand() % 52);
            for (k = 0; k < (int)known_pokers.size(); ++k)
            {
                if(i == known_pokers[k].GetInitNum())
                {
                    flag = false;
                    break;
                }
            }

            if(flag)
            {
                add(Poker(i));
                known_pokers.push_back(Poker(i));
                count++;
            }
        }
    }
    /* 将other中的牌添加到 当前手牌中 */
    void GetFromOther(HandCards other)
    {
        vector<Poker> hand = other.GetData();
        vector<Poker>::iterator t;
        for (t=hand.begin(); t!=hand.end(); t++) {
            add(Poker(t->GetNum()));
        }
    }
   /* 打印手牌 */
    void print()
    {
        int n = data.size();
        int *hand = new int[n];
        for (int i=0; i<n; ++i)
            hand[i]=data[i].data;
        print_hand(hand,n);
        printf("\n");
        delete []hand;
    }

    // 但手牌大于等于 5张时保留最大的组合
    void CalcMax()
    {
       if (data.size()<5)
       {
             cout<<"error! The number of handcards must be more than 4!"<<endl;
             exit(-1);
       }

       data = ChooseMax(data);
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

    bool operator<(const HandCards & t) const
    {
       if (data.size()!=5 || t.data.size()!=5)
       {
             cout<<"error! The number of handcards must be equal to 5!"<<endl;
             exit(-1);
       }

       return GetDistinct()>t.GetDistinct();
    }
    
    bool operator>(const HandCards & t) const
    {
        if (data.size()!=5 || t.data.size()!=5)
        {
            cout<<"error! The number of handcards must be equal to 5!"<<endl;
            exit(-1);
        }
        
        return GetDistinct()<t.GetDistinct();
    }
    
    bool operator>=(const HandCards & t) const
    {
        if (data.size()!=5 || t.data.size()!=5)
        {
            cout<<"error! The number of handcards must be equal to 5!"<<endl;
            exit(-1);
        }
        
        return GetDistinct()<=t.GetDistinct();
    }

};

class Board // 游戏牌局
{
public:
    int pot; //注池总量
    int players; //玩家人数
    HandCards mine; //自己的手牌
    HandCards community; //当前公共牌的情况
    HandCards all; //自己手牌+公共牌
    
    Board(){}
    
    void update_pot(int new_pot)
    {
        pot = new_pot;
    }
    
    int get_pot()
    {
        return pot;
    }
    
    void update_players(int new_players)
    {
        players=new_players;
    }
    
    void add_community(SuitOfPoker color, RankOfPoker point)
    {
        community.add(Poker(color,point));
        all.add(Poker(color, point));
    }
    
    void add_community(int no)
    {
        community.add(Poker(no));
        all.add(Poker(no));
    }
    
    HandCards get_community()
    {
        return community;
    }
    
    void add_mine(SuitOfPoker color, RankOfPoker point)
    {
        mine.add(Poker(color, point));
        all.add(Poker(color, point));
    }
    
    void add_mine(int no)
    {
        mine.add(Poker(no));
        all.add(Poker(no));
    }
    
    HandCards get_mine()
    {
        return mine;
    }
    
    
    /*
     计算Hand Strength (HS)
     根据不同时期公共牌(0,3,4,5)的具体情况，补齐公共牌使所有已知牌(手牌2+已知公共牌+剩余公共牌)都是7张
     适用于翻牌前以及翻牌后3、4、5张公共牌的全部情况
     只利用一个对手即可，计算自己的5张牌>=对手的次数
     任何时期根据仅有的自己底牌和公共牌情况，随机取样对手的底牌，得到HS
     */
    double get_hand_strength()
    {
        int win = 0, round;
        vector<Poker> known_cards;
        known_cards = all.GetData();
        HandCards enemy;
        
        HandCards copy_community;
        copy_community.GetFromOther(community);//现有公共牌
        
        HandCards copy_mine;
        copy_mine.GetFromOther(mine);//底牌
        
        int missing_community= 5 - community.GetData().size();//还需补足的剩余公共牌个数，可以取5, 2, 1, 0
        
        for (round=0; round<1000; round++)
        {
            copy_community.Shuffle(missing_community, known_cards);
//            copy_community.print();
            
            copy_mine.GetFromOther(copy_community);
//            copy_mine.print();
            copy_mine.CalcMax();
//            copy_mine.print();
            
            enemy.Shuffle(2, known_cards);
            enemy.GetFromOther(copy_community);
//            enemy.print();
            enemy.CalcMax();
//            enemy.print();
            
            if(copy_mine >= enemy)
                win++;
            
            known_cards.clear();
            known_cards=all.GetData();
            
            copy_mine.clear();
            copy_mine.GetFromOther(mine);
            
            copy_community.clear();
            copy_community.GetFromOther(community);
            
            enemy.clear();
        }
        
        double HS=(double)win/(double)round;
        cout<<"Hand Strength: "<<HS<<endl;
        return HS;
    }
    
    double calculate_PotOdds(int my_bet)    //计算赔率Pot odds
    {
        return (double)my_bet / (double)(get_pot() + my_bet);
    }
    
    double calculate_RR(int my_bet)     //计算回报率RR
    {
        double rr = get_hand_strength() / calculate_PotOdds(my_bet);
        cout<<"Rate of Return (RR): "<<rr<<endl;
        return rr;
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

 vector<Poker> ChooseMax( vector<Poker> &data)
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

/*
    original 自己原始手牌
    community 公共牌，数量从 3 到 5 张
    N  要凑齐几张牌来比较， 例如 2张手牌 + 3 张公共牌 需要再随机两张牌凑齐7张，所以N=7
    N_enemy 对手的数量
    tar_round  随机取样的次数
    Result 用于保存自己手牌的类型统计结果

Return:
    预期排名，数值越小越好
*/
double calculate_hand_strength(HandCards original,
                               HandCards community,
                               int N,
                               int N_enemy,
                               int tar_round,
                               Result *_res = NULL
                               )
{
     srand((int)time(NULL));
     vector<Poker> known_cards,temp;

     // 计算已知牌列表
     known_cards = original.GetData();
     for (int i=0; i<(int)community.data.size(); ++i)
        known_cards.push_back(community.data[i]);

     temp = known_cards;  //列表备份
     int win = 0;
    
      for (int round=0; round < tar_round; round++)
      {
          HandCards mine;

          if (community.GetData().size()==0)
          {
              community.Shuffle(3, known_cards);
          }

          vector<HandCards> enemy(N_enemy,community);

          mine.data = known_cards;
          mine.Shuffle(N - (int)known_cards.size(), known_cards);
          mine.CalcMax();

          if (_res)
          {
              int a = mine.GetUnique();
              int b = mine.GetDistinct();
              int c = mine.GetClassRank();
              _res->add(a,b,c);
          }

//          mine.print();
          for (int j=0;j<N_enemy;++j)
          {
              enemy[j].Shuffle(N - (int)enemy[j].GetData().size(), known_cards);
              enemy[j].CalcMax();
          }
          
          sort(enemy.begin(),enemy.end());

//          enemy.print();
         int p = N_enemy + 1;
         for (int j=0;j<N_enemy;++j)
         {
            if  (mine < enemy[N_enemy-j-1])
                continue;
            p = j+1;
            break;
         }
          
          win += p;
          known_cards = temp;
      }

    return (double)win/tar_round;

}

class Init
{
public:

    Init()
    {
        cout<<"System is initiated!"<<endl;
        srand((int)time(NULL));
        SystemInit();
    }
} hgsdfjhgyu;

#endif // UTIL_H_INCLUDED
