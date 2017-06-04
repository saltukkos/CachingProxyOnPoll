//
// Created by saltuk on 12/9/16.
//

#ifndef PROXY_CACHEMANAGER_H
#define PROXY_CACHEMANAGER_H


#include "CacheEntry.h"
#include <map>

class CacheManager {
private:
    static constexpr long maxSize = 1024*1024*300;
    static const double garbageLimit;

    long currentSize = 0;
    long lastUse = 0;

    std::map<std::string, CacheEntryPtr> map;

    CacheEntry* createNode();
public:
    CacheEntryPtr getEntry(std::string key);
    void detachEntry(std::string key);

    CacheManager() = default;
    CacheManager(CacheManager&) = delete;
    CacheManager& operator=(CacheManager&) = delete;
};

#endif //PROXY_CACHEMANAGER_H
