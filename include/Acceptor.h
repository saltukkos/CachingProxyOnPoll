//
// Created by saltuk on 12/9/16.
//

#ifndef PROXY_ACCEPTOR_H
#define PROXY_ACCEPTOR_H

#include "Proxy.h"
#include "Pollable.h"
#include "CacheManager.h"

class Acceptor : public Pollable {
private:
    int fd;
public:
    Acceptor(int port);
    Acceptor(Acceptor &other) = delete;
    Acceptor& operator=(Acceptor &other) = delete;

    virtual bool handlePoll(int op, PollStoragePtr storage, CacheManager &cacheManager) override;

    virtual int getFileDescriptor() const override;

    virtual ~Acceptor() override;

    virtual short getOperations() const override;
};


#endif //PROXY_ACCEPTOR_H
