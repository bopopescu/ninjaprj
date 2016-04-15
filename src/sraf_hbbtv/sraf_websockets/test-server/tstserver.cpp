#include "websocketserver.h"
#include <map>
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "pthread.h"
#include <sys/syscall.h>
#define gettid() syscall(__NR_gettid)  
#include <sys/types.h>

using namespace std;

class MultyClient: public Client{
    public:
    virtual void onConnect(){
        printf("----client onConnect fd=%d url=%s\n",getCurrentFd(),getUrl());
	    char buf[50];
        sprintf(buf,"onConne welcome ,client %d\n",getCurrentFd());
        sendMessage(buf,strlen(buf));
        wsmap.insert(std::pair<int,struct lws*>(getCurrentFd(),mCurrentWsi));
    }
    virtual void onMessage(unsigned char*msg,int len){
        unsigned char m[30]={0};
        memcpy(m,msg,len);
        printf("---onMessage len=%d m=%s\n",len,m);

	    char bufm[50];
        sprintf((char*)bufm,"welcome ,client %d\n",getCurrentFd());
        std::map<int,struct lws*>::iterator it;
        for(it=wsmap.begin();it!=wsmap.end();it++){
            printf("sending to %d\n",it->first);
            sendMessage(bufm,strlen(bufm),it->second);
        }

        char ip[50];
        getClientIp(ip,sizeof(ip));
        lwsl_notice("----onMessage from %s\n", ip);

    };
    virtual void onClose(){
       // printf("---client %d onClose\n",getCurrentFd());
        wsmap.erase(getCurrentFd());
    };

    void sendAll(){
	    char bufm[50];
        sprintf((char*)bufm,"welcome ,client %d\n",getCurrentFd());
        std::map<int,struct lws*>::iterator it;
        for(it=wsmap.begin();it!=wsmap.end();it++){
            printf("sending to %d\n",it->first);
            sendMessage(bufm,strlen(bufm),it->second);
        }

    }

    std::map<int,struct lws*>wsmap;
    
};

MultyClient* _mclient;
void *hello(void *ptr) {
    _mclient=new MultyClient();
    startWebSocketServer(7681,_mclient);
    return 0;
}
int main(){
    pthread_t thread;
    pthread_create(&thread, NULL, hello, NULL);
    printf("-----next main sleep 5\n");
    sleep(15);
    printf("-----fini main sleep 5,next notify all client\n");
    _mclient->sendAll();

    pthread_join(thread, NULL);
    return 0;
}
