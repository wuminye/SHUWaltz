#include "Util.h"



void test()
{
    /*
       一张牌的类Poker
       两种构造函数
       Poker a(ace,spade);
        Poker a(12);
{spade, heart,  diamond,club};
{deuce,trey,four,five,six,seven,eight,
 nine,ten,jack,queen,king,ace};
    */
    Poker a(jack,diamond);
    Poker b(nine,spade);


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

    //凑齐7张牌进行分析
    Result res = t.Analyze(7);
    //分析结果
    res.Calc();
    //显示结果
    res.show();
}


int main()
{


   test();

}
