#include "Util.h"

void test()
{
    /*
       一张牌的类Poker
       两种构造函数
        Poker a(ace,spade);
        Poker a(12);
     {spade,heart,diamond,club};
     {deuce,trey,four,five,six,seven,eight,nine,ten,jack,queen,king,ace};
     
    */
    
//    Poker a(jack,diamond);
//    Poker b(nine,spade);
    Poker a(17);
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
    printf("%d",a.GetNum());
//    t.add(b);
    //凑齐7张牌进行分析
//    Result res = t.Analyze(7);
//    //分析结果
//    res.Calc();
//    //显示结果
//    res.show();
}

void test2()
{
  for(int i=0;i<13*4-1;++i){
    for(int j=i+1;j<13*4-1;++j){
      HandCards t;
      t.add(Poker(i));
      t.add(Poker(j));
      Result res = t.Analyze(7);
      //分析结果
      res.Calc();
      //显示结果
      res.show();
    }
  }
}

void calculate_hand_strength()
{
    int win = 0, round;
    vector<Poker> known_cards;
    HandCards mine,enemy,community;
    mine.Shuffle(2, known_cards);
    mine.print();

    HandCards original;
    original.GetFromOther(mine);
    
    for (round=0; round<1000; round++)
    {
        community.Shuffle(3, known_cards);
        community.print();
        
        mine.GetFromOther(community);
        mine.print();
        
        enemy.Shuffle(2, known_cards);
        enemy.GetFromOther(community);
        enemy.print();
        
        if(mine.GetDistinct()<=enemy.GetDistinct())
            win++;
        
        known_cards.clear();
        known_cards.push_back(mine.GetData()[0]);
        known_cards.push_back(mine.GetData()[1]);
        
        community.clear();
        enemy.clear();
        mine.clear();
        mine.GetFromOther(original);
    }
    
    cout<<"底牌为：";
    mine.print();
    cout<<"的大致胜率为"<<(double)win/(double)round;

    
}

int main()
{
//    freopen("/Users/rydge/desktop/in.txt","w",stdout);
//    test();
    calculate_hand_strength();
}
