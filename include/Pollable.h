//
// Created by saltuk on 12/8/16.
//

#ifndef PROXY_SELECTABLE_H
#define PROXY_SELECTABLE_H

#include "PollStorage.h"
#include "CacheManager.h"

class Pollable{
public:
    virtual ~Pollable() {};
    virtual bool handlePoll(int op, PollStoragePtr storage, CacheManager &cacheManager) = 0;
    virtual int getFileDescriptor() const = 0;
    virtual short getOperations() const = 0;
};

#endif //PROXY_SELECTABLE_H
