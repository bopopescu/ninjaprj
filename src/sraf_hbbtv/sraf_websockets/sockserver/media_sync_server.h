#ifndef SRAF_MEDIA_SYNC_SERVER
#define SRAF_MEDIA_SYNC_SERVER

#include "websocketserver.h"
#include <map>
#include <vector>

class MediaSyncServer;

class ClientObserver {
    public:
        virtual void onNewClient(char* requrl,int clientid){
        }
        virtual void onMessage(unsigned char*msg,int len,int clientid){
        }
        virtual void onClientLeave(int clientid){
        }
        void sendMessage(char* msg,int len,int toClientId);
        void sendMessageAll(char* msg,int len,int excludeId);

        void setServer(MediaSyncServer* mediaserver){mServer=mediaserver;}

    private :
        MediaSyncServer* mServer;
};

class MediaSyncServer: public Client{
    public:
    MediaSyncServer();
    virtual void onConnect();
    virtual void onMessage(unsigned char*msg,int len);
    virtual void onClose();
    int observerSendMessage(char* msg,int len,int toClientId);
    int observerSendMessageAll(char* msg,int len,int excludeId);
    void addObserver(ClientObserver* observer);
    
    private:
    std::map<int,struct lws*>wsmap;
    std::map<int,int>fdIdMap;
    std::vector<ClientObserver*> mObserverList;
    int mClientIdIndex;
    
};

#endif
