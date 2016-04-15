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

class ForwardClientObserver: public ClientObserver {
    public:
        virtual void onNewClient(char* requrl,int clientid){
            printf("ForwardClientObserver::onNewClient url=%s,id=%d\n",requrl,clientid);
        }
        virtual void onMessage(unsigned char*msg,int len,int clientid){
            printf("ForwardClientObserver::onMessage msg=%s,from=%d\n",msg,clientid);
            sendMessageAll((char*)msg,len,clientid);
        }
        virtual void onClientLeave(int clientid){
            printf("ForwardClientObserver::onClientLeave from=%d\n",clientid);
        }
};


MediaSyncServer* _mclient;
ClientObserver* _clientObs;
void *startServer(void *ptr) {
    _clientObs=new ForwardClientObserver();
    _mclient=new MediaSyncServer();
    _mclient->addObserver(_clientObs);
    startWebSocketServer(7681,_mclient);
    return 0;
}
int main(){
    pthread_t thread;
    pthread_create(&thread, NULL, startServer, NULL);
    printf("-----next main sleep 5, tid=%d\n",(int)thread);
    sleep(15);

    void *end;
    pthread_join(thread, &end);
    return 0;
}
