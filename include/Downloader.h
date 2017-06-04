//
// Created by saltuk on 12/9/16.
//

#ifndef PROXY_DOWNLOADER_H
#define PROXY_DOWNLOADER_H


#include "Pollable.h"
#include "Request.h"
#include "CacheEntry.h"
#include "CacheManager.h"

class Downloader : public Pollable {
private:
    static const int bufSize = 1024*10;
    const Request request;
    ssize_t requestPosition;
    int fd;
    short ops;
    CacheEntryPtr cache;
    char *buf;
public:

    Downloader(const Request request, CacheEntryPtr entry);
    Downloader& operator=(Downloader&) = delete;
    virtual ~Downloader() override;

    virtual bool handlePoll(int op, PollStoragePtr storage, CacheManager &cacheManager) override;

    virtual int getFileDescriptor() const override;

    virtual short getOperations() const override;

};


#endif //PROXY_DOWNLOADER_H
