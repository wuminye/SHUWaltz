#include "Util.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <vector>
#include <string>
#include <iostream>
#include "WorksCore.h"
#include "MessageProcess.h"
using namespace std;

int main(int argc, char *argv[])
{
    if(argc!=6){
      printf("Usage: ./%s server_ip server_port my_ip my_port my_id\n",argv[0]);
      return -1;
    }
   core = new Core(argc, argv);
   while (true)
   {
         //获取队列消息
         string res = core-> GetMSG();
         if (res != "#NULL#")
         {

          /*
            if(!process_sever_message(core,res.length(),res.c_str())){
              core->CloseThread();
            }
            cout<<"[string]:\n"<<res<<endl;
            printf("---------------------\n");
            */

            if ((res.length()>10) && strstr(res.c_str(),"inquire/")!= NULL)
            {
               const char* rep_msg = "all_in\n";
               core->sendmesg(rep_msg,0);
            }

         }
         usleep(1000);
         if (!core->IsThreadRunning())
            break;
   };
   core->CloseThread();
   pthread_join (core->recvT,NULL);
   delete core;
}
