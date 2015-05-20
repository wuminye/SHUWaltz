#include <netinet/in.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <cstdio>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <queue>
using namespace std;



class NetCore
{
public:
  in_addr_t server_ip;
  in_port_t server_port;
  in_addr_t my_ip;
  in_port_t my_port;
  sockaddr_in my_addr;
  sockaddr_in server_addr;
  int socket_id;
  ~NetCore()
  {
    close(socket_id);
    printf("sockets closed.\n");
  }
  int Init()
  {
     socket_id = socket(AF_INET,SOCK_STREAM,0);
     my_addr.sin_family=AF_INET; //设置为IP通信
     my_addr.sin_addr.s_addr=my_ip;//设置ip
     my_addr.sin_port=htons(my_port);//设置端口

     long flag=1;
     setsockopt(socket_id,SOL_SOCKET,SO_REUSEADDR,(char *)&flag,sizeof(flag));
     if(bind(socket_id,(sockaddr*)&my_addr,sizeof(sockaddr))<0){
         printf("bind fail.\n");
         return -1;
     }

     server_addr.sin_family=AF_INET; //设置为IP通信
     server_addr.sin_addr.s_addr=server_ip;//设置ip
     server_addr.sin_port=htons(server_port);//设置端口

     while(connect(socket_id, (sockaddr*)&server_addr, sizeof(sockaddr))!=0){
         usleep(100*1000);//挂起100ms
     }
     printf("connect server success.\n");

    return 0;
  }

  ssize_t sendmesg(const char *buff,  int flags = 0)
  {
    /*
    char * buf = new char[strlen(buff)+3];
    strcpy(buf,buff);
    int n = strlen(buff);
    buf[n] = '\n';
    buf[n+1] = '\0';
*/
    ssize_t res = send(socket_id,buff,strlen(buff),flags);

  //  delete []buf;
    return res;
  }


};

int CRecvThread;
class Core;

void *RecvThread(void *c);



class Core
{
public:
  int my_id;
  string my_name;
  NetCore networks;
  queue<string> mesgQ;
  pthread_t recvT;
  pthread_mutex_t mutex_x;

  Core(int argc, char *argv[])
  {
     mutex_x = PTHREAD_MUTEX_INITIALIZER;
     CRecvThread = 1;

    //根据参数提取信息
     networks.server_ip = inet_addr(argv[1]);
     networks.server_port = atoi(argv[2]);
     networks.my_ip = inet_addr(argv[3]);
     networks.my_port = atoi(argv[4]);
     my_id = atoi(argv[5]);

     char *p = strrchr(argv[0],'/');
     if(p==NULL){
         p = argv[0];
     }
     else{
         p++;
     }
     my_name = string(p);

     if (networks.Init()<0)
     {
         printf("网络初始化错误!\n");
         exit(-1);
     }

     char reg_msg[50]="";
     snprintf(reg_msg,sizeof(reg_msg)-1, "reg: %d %s \n",my_id,my_name.c_str());
     printf("[send register message]: %s",reg_msg);
     networks.sendmesg(reg_msg);

     if (pthread_create(&recvT,NULL,RecvThread,this)!=0)
     {
         printf("Create thread  error!\n");
         exit(1);
     }
  }

  void CloseThread()
  {
    CRecvThread=0;
  }
  bool IsThreadRunning()
  {
     return CRecvThread;
  }
  void ProcessMSG(char *data)
  {

    while (data[0]!='\0' && data[0]!='\n')
    {
       char *p = strstr(data,"/");

       if (p==NULL)
       {
         pthread_mutex_lock(&mutex_x);

         mesgQ.push(string(data,strlen(data)));

         pthread_mutex_unlock(&mutex_x);

         return;
       }

       int l = p - data + 1;
       char *p2 = strstr(p+1,"/");
       p2 += l; //eol


       pthread_mutex_lock(&mutex_x);

       mesgQ.push(string(data,p2-data+1));

       pthread_mutex_unlock(&mutex_x);
       data = p2+1;

    }
  }

  string GetMSG()
  {
     string res;
     pthread_mutex_lock(&mutex_x);

     if (mesgQ.size()==0)
         res = "#NULL#";
     else
     {
       res =mesgQ.front();
       mesgQ.pop();
     }


     pthread_mutex_unlock(&mutex_x);

     return res;
  }


}* core;

void *RecvThread(void *c)
{
   Core * core = (Core *)c;
   printf("Recver Setup.\n");
   while (CRecvThread)
   {
      char buffer[4096]={"\0"};
      int recv_size = recv(core->networks.socket_id,buffer,sizeof(buffer)-1,0);
      if (recv_size <=0)
         break;
      if(recv_size>0)
      {
          core->ProcessMSG(buffer);
          //printf("[recv]: \n%s",buffer);
      }
   }
   CRecvThread = 0;
   printf("Recver Over.\n");
   return NULL;
}
