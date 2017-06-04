//
// Created by saltuk on 12/9/16.
//

#ifndef PROXY_HTTPPROXY_H
#define PROXY_HTTPPROXY_H


#include <vector>
#include <poll.h>
#include "Pollable.h"
#include "PollStorage.h"

class HTTPProxy {
private:
    PollStoragePtr storage;
    CacheManager cacheManager;
public:
    HTTPProxy(int port = 4242);
    void run();
};


#endif //PROXY_HTTPPROXY_H
