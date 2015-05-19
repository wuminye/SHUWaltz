﻿#include "Util.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

Board board;

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
    if(board.get_chip()-board.get_last_bet()<board.get_blind()*4 && board.get_hand_strength()<0.5)
    {
        //弃牌fold动作
    }
        
}

/* 处理server的消息 */
bool process_sever_message(int socket_id, int size, const char* msg){
  printf("收到消息: %s\n",msg);
    
  if(strstr(msg, "game-over")!=NULL)//game over
  {
    return false;
  }

  if(strstr(msg, "inquire/"))
  {
      //得到当前底池的总金额，具体值根据server
      board.update_pot(1000);
      
      //同时得到其他玩家的行为，加入决策
      //TO-DO
      
      board.update_last_bet(20);//假设20
      //按决策进行相应的action
      FCR_decision(board.get_last_bet());
      
      /*
       发送行动消息(action-msg)
       check | call | raise num | all_in | fold eol
       */
      const char* rep_msg = "all_in";
      send(socket_id,rep_msg,(int)strlen(rep_msg)+1,0);
  }
   
    /*
     
     公牌消息(flop-msg) server发出三张公牌
     flop/ eol
     color point eol 
     color point eol
     color point eol
     /flop eol
     
     */
    if(strstr(msg, "flop/"))
    {
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
    if(strstr(msg, "hold/"))
    {
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
    if(strstr(msg,"turn/"))
    {
        board.add_community(DIAMONDS,ace);
    }
    
    /*
     
     河牌消息(river-msg)
     ￼river/ eol 
     color point eol 
     /river eol
     
     */
    if(strstr(msg, "river/"))
    {
        board.add_community(DIAMONDS,ace);
    }
    
    return true;

}

int main(int argc, char *argv[])
{
//    if(argc!=6){
//        printf("Usage: ./%s server_ip server_port my_ip my_port my_id\n",argv[0]);
//        return -1;
//    }
//    //根据参数提取信息
//    in_addr_t server_ip = inet_addr(argv[1]);
//    in_port_t server_port = atoi(argv[2]);
//    in_addr_t my_ip = inet_addr(argv[3]);
//    in_port_t my_port = atoi(argv[4]);
//    int my_id = atoi(argv[5]);
//    
//    char *my_name = strrchr(argv[0],'/');
//    if(my_name==NULL){
//        my_name = argv[0];
//    }
//    else{
//        my_name++;
//    }
//    
//    int socket_id = socket(AF_INET,SOCK_STREAM,0);
//    
//    sockaddr_in my_addr;
//    my_addr.sin_family=AF_INET; //设置为IP通信
//    my_addr.sin_addr.s_addr=my_ip;//设置ip
//    my_addr.sin_port=htons(my_port);//设置端口
//    long flag=1;
//    setsockopt(socket_id,SOL_SOCKET,SO_REUSEADDR,(char *)&flag,sizeof(flag));
//    if(bind(socket_id,(sockaddr*)&my_addr,sizeof(sockaddr))<0){
//        printf("bind fail.\n");
//        return -1;
//    }
//    
//    sockaddr_in server_addr;
//    server_addr.sin_family=AF_INET; //设置为IP通信
//    server_addr.sin_addr.s_addr=server_ip;//设置ip
//    server_addr.sin_port=htons(server_port);//设置端口
//    
//    while(connect(socket_id, (sockaddr*)&server_addr, sizeof(sockaddr))!=0){
//        usleep(100*1000);//挂起100ms
//    }
//    printf("connect server success.\n");
//    
//    char reg_msg[50]="";
//    snprintf(reg_msg,sizeof(reg_msg)-1, "reg: %d %s \n",my_id,my_name);
//    printf("send register message%s",reg_msg);
//    send(socket_id,reg_msg,(int)strlen(reg_msg)+1,0);
//    
//    while(true){
//        char buffer[1024]={"\0"};
//        int recv_size = recv(socket_id,buffer,sizeof(buffer)-1,0);
//        if(recv_size>0){
//            if(!process_sever_message(socket_id,recv_size,buffer)){
//                break;
//            }
//        }
//    }
//    close(socket_id);
    
    test4();
    test5();
    test6();
    test7();
}
