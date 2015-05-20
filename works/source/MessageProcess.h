#ifndef MESSAGEPROCESS_H
#define MESSAGEPROCESS_H
#include "Util.h"
#include <string>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <cstdio>
#include "WorksCore.h"

Board board;

vector <string> split_msg(int size,const char *msg){
  /*
  split message by eofl
  return a vector contain every line
  */
    vector <string> res;
    string temp="";
    for(int i=0;i<size;++i){
        if(msg[i]=='\n'){
            res.push_back(temp);
            temp="";
        }
        else if(i==size-1){
            temp+=msg[i];
            res.push_back(temp);
        }
        else{
            temp+=msg[i];
        }
    }
    return res;
}
void test()
{
    /*
       一张牌的类Poker
       两种构造函数
        Poker a(SPADES,ace);
        Poker a(12);
     {SPADES,HEARTS,DIAMONDS,CLUBS};

     [2-10]|J|Q|K|A

    */

    Poker a(DIAMONDS,jack);
    Poker b(SPADES,nine);
    Poker c(17);

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
    t.add(b);
    //凑齐7张牌进行分析
    Result res = t.Analyze(7);
    //分析结果
    res.Calc();
    //显示结果
    res.show();
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
    Result *res = new Result();

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

//翻牌前
void test4()
{
    cout<<"==========翻牌前测试=========="<<endl;
    vector<Poker> known_cards;
    board.mine.Shuffle(2, known_cards);
    cout<<"底牌:"<<endl;
    board.mine.print();
    board.update_pot(40); //现在注池有40
    board.calculate_RR(20); //计算假设跟注后的RR，根据RR决定action

    board.mine.clear();
    board.community.clear();
}

//三张公共牌
void test5()
{

    cout<<"==========翻牌局测试=========="<<endl;
    vector<Poker> known_cards;


    board.mine.Shuffle(2, known_cards);
    cout<<"底牌:"<<endl;
    board.mine.print();
    board.community.Shuffle(3, known_cards);
    cout<<"三张公共牌:"<<endl;
    board.community.print();
    board.update_pot(40); //现在注池有40
    board.calculate_RR(20); //计算假设跟注后的RR，根据RR值进行评估

    board.mine.clear();
    board.community.clear();
}

//四张公共牌
void test6()
{

    cout<<"==========转牌局测试=========="<<endl;
    vector<Poker> known_cards;
    board.mine.Shuffle(2, known_cards);
    cout<<"底牌:"<<endl;
    board.mine.print();
    board.community.Shuffle(4, known_cards);
    cout<<"四张公共牌:"<<endl;
    board.community.print();
    board.update_pot(40); //现在注池有40
    board.calculate_RR(20); //计算假设跟注后的RR，根据RR值进行评估


    board.mine.clear();
    board.community.clear();
}

//五张公共牌
void test7()
{

    cout<<"==========河牌局测试=========="<<endl;
    vector<Poker> known_cards;
    board.mine.Shuffle(2, known_cards);
    cout<<"底牌:"<<endl;
    board.mine.print();
    board.community.Shuffle(5, known_cards);
    cout<<"五张公共牌:"<<endl;
    board.community.print();
    board.update_pot(40); //现在注池有40
    board.calculate_RR(20); //计算假设跟注后的RR，根据RR值进行评估

    board.mine.clear();
    board.community.clear();

}


/*
 FOLD/CALL/RAISE DECISION
 If RR < 0.8 then 95% fold, 0 % call, 5%  raise (bluff)
 If RR < 1.0 then 80% fold, 5%  call, 15% raise (bluff)
 If RR < 1.3 then 0%  fold, 60% call, 40% raise
 Else (RR >= 1.3) 0%  fold, 30% call, 70% raise
 */

void FCR_decision(int my_bet)//TO-DO
{
    double rr= board.calculate_RR(my_bet); //RR回报率 Rate of Return = Hand Strength / Pot Odds.
    if(rr<0.8)
    {

    }
    else if(rr<1.0)
    {


    }
    else if(rr<1.3)
    {

    }
    else // rr >=1.3
    {

    }

}
/*

 筹码保护
 当自己的筹码很少时执行
 如果叫牌会让你只剩下不到四倍的盲注，那就不用叫牌，除非你有50%以上的胜算

 */
void stack_protection()
{
    //if (stack- bet) < (blind * 4) and (HS < 0.5) then fold
    //如果（筹码-下注）<(盲注*4)并且（HS<0.5）那么就弃牌
    if(board.get_my_chip()-board.get_last_bet()<board.get_blind()*4 && board.get_hand_strength()<0.5)
    {
        //执行弃牌fold动作
    }

}

bool process_sever_message(Core *core, int size, const char* msg){
  /*
  process single message receive from server
  */
  if(size<=0||strlen(msg)==0)return true;
  printf("receive message from server:\n %s\n",msg);
  vector<string> splited_msg = split_msg(size,msg);
  int msg_lines = splited_msg.size();
  if(strstr(msg, "game-over")!=NULL)//game over
  {
    return false;
  }
  else if (strstr(msg,"seat/")){
    //位置消息，表示开始新的一局，清空所有vector
    //clearn board
    board.clear();
    board.update_players(msg_lines-2);
    for(int i=0;i<msg_lines;++i){
      string temp = splited_msg[i];
      std::string::size_type pos;
      string ext_msg;
      int pid;//用户id
      int jetton;//当前筹码
      int money;//当前金币
      if((pos=temp.find("seat"))!=std::string::npos)continue;
      else if((pos=temp.find("button:"))!=std::string::npos){
        //找到庄家
        ext_msg = temp.substr(pos+strlen("button:"));
        std::sscanf(ext_msg.c_str(),"%d %d %d",&pid,&jetton,&money);
        printf("button %d have %d jetton %d money\n",pid,jetton,money);
      }
      else if((pos=temp.find("small blind:"))!=std::string::npos){
        //小盲注
        ext_msg = temp.substr(pos+strlen("small blind:"));
        std::sscanf(ext_msg.c_str(),"%d %d %d",&pid,&jetton,&money);
        printf("small %d have %d jetton %d money\n",pid,jetton,money);
      }
      else if((pos=temp.find("big blind:"))!=std::string::npos){
        //大盲注
        ext_msg = temp.substr(pos+strlen("big blind:"));
        std::sscanf(ext_msg.c_str(),"%d %d %d",&pid,&jetton,&money);
        printf("big %d have %d jetton %d money\n",pid,jetton,money);
      }
      else{
        std::sscanf(temp.c_str(),"%d %d %d",&pid,&jetton,&money);
        printf("user %d have %d jetton %d money\n",pid,jetton,money);
      }
    }
  }
  else if(strstr(msg,"blind/")){
    /*盲注消息
    blind/ eol
    pid: bet(1 or 2 lines)
    /blind eol
    */
    int pid;
    int blind_bet;
    for(int i=0;i<msg_lines;++i){
      if(splited_msg[i].find("blind")!=std::string::npos)continue;
      std::sscanf(splited_msg[i].c_str(),"%d:%d",&pid,&blind_bet);
      printf("user %d blind %d\n",pid,blind_bet);
    }
  }
  else if(strstr(msg, "inquire/"))
  {
      //得到当前底池的总金额，具体值根据server
      board.update_pot(1000);

      //同时得到其他玩家的行为，加入决策
      //TO-DO

      board.update_last_bet(20);//假设20
      //按决策进行相应的action

      /*
       发送行动消息(action-msg)
       check | call | raise num | all_in | fold eol
       */
      const char* rep_msg = "all_in\n";
      core->sendmesg(rep_msg,0);
      printf("\n[send]: \n%s\n",rep_msg);
  }

    /*

     公牌消息(flop-msg) server发出三张公牌
     flop/ eol
     color point eol
     color point eol
     color point eol
     /flop eol

     */
    else if(strstr(msg, "flop/"))
    {
      char color[3][20];
      char point[3][20];
      int num=0;
      for(int i=0;i<msg_lines;++i){
        if(splited_msg[i].find("flop")!=std::string::npos)continue;
        std::sscanf(splited_msg[i].c_str(),"%s%s",color[num],point[num]);
        printf("get flop card with color:%s and point:%s\n",color[num],point[num]);
        num++;
      }
        //添加三张公牌信息，具体值根据server来定
        board.add_community(DIAMONDS,ace);
        board.add_community(DIAMONDS, ace);
        board.add_community(DIAMONDS, ace);
    }

    /*

     手牌消息(hold-cards-msg)
     hold/ eol
     color point eol
     color point eol
     /hold eol

     */
    else if(strstr(msg, "hold/"))
    {
      char color[20];
      char point[20];
      for(int i=0;i<msg_lines;++i){
        if(splited_msg[i].find("hold")!=std::string::npos)continue;
        std::sscanf(splited_msg[i].c_str(),"%s%s",color,point);
        printf("get hold cards with color:%s and point:%s\n",color,point);
      }
      //初始化两张底牌信息，具体值根据server
      board.add_mine(SPADES, ace);
      board.add_mine(DIAMONDS,king);
    }

    /*

     转牌消息(turn-msg)
     turn/ eol
     color point eol
     /turn eol

     */
    else if(strstr(msg,"turn/"))
    {
      char color[20];
      char point[20];
      for(int i=0;i<msg_lines;++i){
        if(splited_msg[i].find("turn")!=std::string::npos)continue;
        std::sscanf(splited_msg[i].c_str(),"%s%s",color,point);
        printf("get turn card with color:%s and point:%s\n",color,point);
      }
      board.add_community(DIAMONDS,ace);
    }

    /*

     河牌消息(river-msg)
     ￼river/ eol
     color point eol
     /river eol

     */
    else if(strstr(msg, "river/"))
    {
      char color[20];
      char point[20];
      for(int i=0;i<msg_lines;++i){
        if(splited_msg[i].find("river")!=std::string::npos)continue;
        std::sscanf(splited_msg[i].c_str(),"%s%s",color,point);
        printf("get river card with color:%s and point:%s\n",color,point);
      }
      board.add_community(DIAMONDS,ace);
    }
    /*
    彩池分配消息
    pot-win/ eol
    pid: num eol
    /pot-win eol
    */
    else if(strstr(msg,"pot-win/")){
      int pid;
      int win_money;
      for(int i=0;i<msg_lines;++i){
        if(splited_msg[i].find("pot-win")!=std::string::npos)continue;
        std::sscanf(splited_msg[i].c_str(),"%d: %d",&pid,&win_money);
        printf("user %d win %d\n",pid,win_money);
      }
    }
    /*
    摊牌消息
    showdown/ eol
    common/ eol
    color point(5 cards)
    /common eol
    rank: pid color point color point nut_hand eol
    /showdown eol
    */
    else if(strstr(msg,"showdown/")){
      std::string::size_type pos;
      string ext_msg;
      int pid;
      int rank;
      char color[20];
      char point[20];
      char color2[20];
      char point2[20];
      char nut_hand[20];//与公共牌组成最大的手牌类型
      bool in_common=false;
      for(int i=0;i<msg_lines;++i){
        string temp=splited_msg[i];
        if((pos=temp.find("showdown"))!=std::string::npos)continue;
        else if((pos=temp.find("common/"))!=std::string::npos){
          in_common=true;
          continue;
        }
        else if((pos=temp.find("/common"))!=std::string::npos){
          in_common=false;
          continue;
        }
        if(in_common){
          std::sscanf(temp.c_str(),"%s %s",color,point);
          printf("common card color:%s point:%s\n",color,point);
        }
        else{
          std::sscanf(temp.c_str(),"%d: %d %s %s %s %s %s",&rank,&pid,color,point,color2,point2,nut_hand);
          printf("user %d hold %s %s and %s %s rank at %d with %s\n",pid,color,point,color2,point2,rank,nut_hand);
        }
      }
    }
    return true;
}
#endif
