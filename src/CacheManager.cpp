//
// Created by saltuk on 12/9/16.
//

#include "CacheManager.h"
#include "Proxy.h"
#include <algorithm>

CacheEntryPtr CacheManager::getEntry(std::string key) {
    CacheEntryPtr &temp = map[key];
    if (!temp.operator bool()) {
        temp.reset(createNode());
#ifdef DEBUG
        std::cerr << "create new cache entry "<< temp.get() <<" for "
                  << key << std::endl;
#endif
    }
    else{
#ifdef DEBUG
        std::cerr << "got cache entry "<< temp.get() << " from cache for "
                  << key << std::endl;
#endif
        }
    temp->setLastUse(lastUse++);
    return temp;
}

void CacheManager::detachEntry(std::string key) {
    map.erase(key);
}

const double CacheManager::garbageLimit = 0.7;

CacheEntry *CacheManager::createNode() {
    if (currentSize >= maxSize){
        typedef std::pair<std::string, CacheEntryPtr> MapEntry;
        std::vector<MapEntry> vector;

        for (auto const& it : map){
            if (it.second)
                vector.push_back(it);
        }

        std::sort(vector.begin(), vector.end(),
                  [](const MapEntry &left, const MapEntry &right){
                      return left.second->getLastUse() < right.second->getLastUse();
                  });

        for (auto& it : vector){
            if (it.second->clientsCount() == 0){
                map.erase(it.first);

                if (currentSize < maxSize*garbageLimit)
                    break;
            }
        }
    }
    if (currentSize >= maxSize)
        throw std::bad_alloc();
    return new CacheEntry(&currentSize);
}
