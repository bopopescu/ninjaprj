#include "websocketserver.h"
#include "media_sync_server.h"
#include <map>
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "pthread.h"
#include <sys/syscall.h>
#define gettid() syscall(__NR_gettid)  
#include <sys/types.h>

using namespace std;

class TestClientObserver: public ClientObserver {
    public:
        virtual void onNewClient(char* requrl,int clientid){
            char msg[]="srvmsg:TestClientObserver::onNewClient";
            sendMessage(msg,sizeof(msg),-1);
            printf("TestClientObserver::onNewClient url=%s,id=%d\n",requrl,clientid);
        }
        virtual void onMessage(unsigned char*msg,int len,int clientid){
            char msgx[]="srvmsg:TestClientObserver::onMessage";
            sendMessage(msgx,sizeof(msgx),-1);
            printf("TestClientObserver::onMessage msg=%s,from=%d\n",msg,clientid);
        }
        virtual void onClientLeave(int clientid){
            printf("TestClientObserver::onClientLeave from=%d\n",clientid);
        }
};


MediaSyncServer* _mclient;
ClientObserver* _clientObs;
void *startServer(void *ptr) {
    _clientObs=new TestClientObserver();
    _mclient=new MediaSyncServer();
    _mclient->addObserver(_clientObs);
    _mclient->addObserver(new TestClientObserver());
    startWebSocketServer(7681,_mclient);
    return 0;
}
int main(){
    pthread_t thread;
    pthread_create(&thread, NULL, startServer, NULL);
    printf("-----next main sleep 5, tid=%d\n",(int)thread);
    sleep(15);
    printf("-----fini main sleep 5,next notify all client\n");
    char msg[]="TestClientObserver:hello all";
    _clientObs->sendMessage(msg,sizeof(msg),-1);

    void *end;
    pthread_join(thread, &end);
    return 0;
}
