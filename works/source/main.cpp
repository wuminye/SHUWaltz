#include "Util.h"



void test()
{
    /*
       一张牌的类Poker
       两种构造函数
       Poker a(ace,spade);
        Poker a(12);
    */
    Poker a(ace,club);
    Poker b(11);
    Poker c(10);
    Poker d(8);
    Poker e(9);

/*
   手牌类 HandCards
   成员函数：
       add(Poker) 添加扑克牌
       GetClass() 获得类别名
       GetUnique() 归类前排名
       GetDistinct() 归类后排名

*/
    HandCards t;
    t.add(a);
    t.add(b);
    t.add(c);
    t.add(d);
    t.add(e);

    cout<<t.GetClass()<<endl;
     cout<<t.GetUnique()<<endl;
     cout<<t.GetDistinct()<<endl;
}


int main()
{


   test();

}
