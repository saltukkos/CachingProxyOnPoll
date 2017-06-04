//
// Created by saltuk on 12/7/16.
//

#ifndef PROXY_CLIENT_H
#define PROXY_CLIENT_H

#include <unistd.h>
#include <fcntl.h>
#include "Pollable.h"
#include "Request.h"
#include "CacheManager.h"

using ClientPtr = std::shared_ptr<class Client>;

class Client : public Pollable{
public:

    enum State{
        WriteToServer, ReadFromServer, Wait, Finish
    };

    Client(int socket);
    Client &operator=(Client&) = delete;

    void notify();

    bool handlePoll(int op, PollStoragePtr, CacheManager &cacheManager) override;

    virtual int getFileDescriptor() const override;

    virtual short getOperations() const override;

    virtual ~Client() override;

private:
    static const int buff_size = 1024*100;
    char *buffer;

    int socket;
    State state;
    Request request;
    int responsePosition;
    CacheEntryPtr entry;
};


#endif //PROXY_CLIENT_H
