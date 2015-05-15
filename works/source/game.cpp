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

// by wuminye
void test3()
{
    int N_enemy = 7;
    vector<Poker> known_cards;
    HandCards mine,community;
    mine.Shuffle(2, known_cards);
    cout<<"手牌:";
    mine.print();
   // community.Shuffle(3, known_cards);
    cout<<"公共牌:";
    community.print();
    Result * res = new Result();

    cout<<"预期排名:"<<calculate_hand_strength(mine,community,7,N_enemy,3000,res)<<"/"<<N_enemy+1<<endl;

    res->Calc();
    cout<<endl<<"预期估计类型概率："<<endl;
    res->show();
    mine.GetFromOther(community);

    Result res2 = mine.Analyze(7);
    res2.Calc();
    cout<<"准确统计类型概率："<<endl;
    res2.show();

    delete res;
}

int main()
{
//    freopen("/Users/rydge/desktop/in.txt","w",stdout);
//    test();
    test3();
}
