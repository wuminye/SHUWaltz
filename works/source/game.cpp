#include "Util.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
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
bool process_message(int socket_id, int size,const char* msg){
  printf("收到消息: %s\n",msg);
  if(strstr(msg, "game-over")!=NULL){//本场比赛结束
    return false;
  }
  if(strstr(msg, "inquire/")){
    const char* rep_msg = "all_in";
    send(socket_id,rep_msg,(int)strlen(rep_msg)+1,0);
  }
  return true;
}
int main(int argc, char *argv[])
{
  if(argc!=6){
    printf("参数个数错误\n");
    return -1;
  }
  //根据参数提取信息
  in_addr_t server_ip = inet_addr(argv[1]);
  in_port_t server_port = atoi(argv[2]);
  in_addr_t my_ip = inet_addr(argv[3]);
  in_port_t my_port = atoi(argv[4]);
  int my_id = atoi(argv[5]);

  char *my_name = strrchr(argv[0],'/');
  if(my_name==NULL){
    my_name = argv[0];
  }
  else{
    my_name++;
  }

  int socket_id = socket(AF_INET,SOCK_STREAM,0);

  sockaddr_in my_addr;
  my_addr.sin_family=AF_INET; //设置为IP通信
  my_addr.sin_addr.s_addr=my_ip;//设置ip
  my_addr.sin_port=htons(my_port);//设置端口

  long flag=1;
  setsockopt(socket_id,SOL_SOCKET,SO_REUSERADDR,(char *)&flag,sizeof(flag));
  if(bind(socket_id,(sockaddr*)&my_addr,sizeof(sockaddr))<0){
    printf("绑定失败\n");
    return -1;
  }

  sockaddr_in server_addr;
  server_addr.sin_family=AF_INET; //设置为IP通信
  server_addr.sin_addr.s_addr=server_ip;//设置ip
  server_addr.sin_port=htons(server_port);//设置端口

  while(connect(socket_id, (sockaddr*)&server_addr, sizeof(sockaddr))!=0){
    usleep(100*1000);//挂起100ms
  }
  printf("连接服务器成功\n");

  char reg_msg[50]="";
  snprintf(reg_msg,sizeof(reg_msg)-1, "reg: %d %s \n",my_id,my_name);
  printf("发送注册消息%s",reg_msg);
  send(socket_id,reg_msg,(int)strlen(reg_msg)+1,0);

  while(true){
    char buffer[1024]={"\0"};
    int recv_size = recv(socket_id,buffer,sizeof(buffer)-1,0);
    if(recv_size>0){
      if(!process_message(socket_id,recv_size,buffer)){
        break;
      }
    }
  }
  close(socket_id);



//    freopen("/Users/rydge/desktop/in.txt","w",stdout);
//    test();
    //test3();
}
