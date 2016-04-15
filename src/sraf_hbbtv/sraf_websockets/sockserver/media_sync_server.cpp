#include "media_sync_server.h"

void ClientObserver::sendMessage(char* msg,int len,int toClientId){
    mServer->observerSendMessage(msg,len,toClientId);
}

void ClientObserver::sendMessageAll(char* msg,int len,int excludeId){
    mServer->observerSendMessageAll(msg,len,excludeId);
}


MediaSyncServer::MediaSyncServer(){
    mClientIdIndex=0;
}

void MediaSyncServer::onConnect(){
    wsmap.insert(std::pair<int,struct lws*>(++mClientIdIndex,mCurrentWsi));
    fdIdMap.insert(std::pair<int,int>(getCurrentFd(),mClientIdIndex)); 
    for(ClientObserver* o : mObserverList){
        o->onNewClient(getUrl(),mClientIdIndex);
    }
}

void MediaSyncServer::onMessage(unsigned char*msg,int len){
    int clientid=fdIdMap.find(getCurrentFd())->second;
    for(ClientObserver* o : mObserverList){
        o->onMessage(msg,len,clientid);
    }

};

void MediaSyncServer::onClose(){
    int clientid=fdIdMap.find(getCurrentFd())->second;
    wsmap.erase(clientid);
    for(ClientObserver* o : mObserverList){
        o->onClientLeave(clientid);
    }
    fdIdMap.erase(getCurrentFd());
};

void MediaSyncServer::addObserver(ClientObserver* observer){
    observer->setServer(this);
    mObserverList.push_back(observer);
}

int MediaSyncServer::observerSendMessage(char* msg,int len,int toClientId){
    if(toClientId!=-1){
        auto res=wsmap.find(toClientId);
        if(res!=wsmap.end()){
            sendMessage(msg,len,res->second);
            return 0;
        }
        return 1;
    }
    std::map<int,struct lws*>::iterator it;
    for(it=wsmap.begin();it!=wsmap.end();it++){
        sendMessage(msg,len,it->second);
    }
    return 0;
}

int MediaSyncServer::observerSendMessageAll(char* msg,int len,int excludeId){
    std::map<int,struct lws*>::iterator it;
    for(it=wsmap.begin();it!=wsmap.end();it++){
        if(it->first!=excludeId){
            sendMessage(msg,len,it->second);
        }
    }
    return 0;

}


