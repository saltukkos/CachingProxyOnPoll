//
// Created by saltuk on 11/25/16.
//

#ifndef PROXY_CACHEENTRY_H
#define PROXY_CACHEENTRY_H


#include <vector>
#include <memory>
#include <iterator>

class Client;
using CacheEntryPtr = std::shared_ptr<class CacheEntry>;

class CacheBackInserter :
        public std::iterator<std::output_iterator_tag,void,void,void,void>
{
private:
    std::vector<char> &vector;
    long *size;
public:
    CacheBackInserter(CacheEntry &x);
    CacheBackInserter& operator= (const char& value);
    CacheBackInserter& operator* ();
    CacheBackInserter& operator++ ();
    CacheBackInserter  operator++ (int);
};

class CacheEntry {
public:
    enum State{
        Empty, Downloading, Finished
    };

    CacheEntry(long *cacheSize);
    CacheEntry(CacheEntry &other) = delete;
    CacheEntry& operator=(CacheEntry&) = delete;
    ~CacheEntry();

    void notifyClients();

    void addClient(Client *client);
    void deleteClient(Client *client);
    unsigned long clientsCount();

    const std::vector<char> &getData() const;
    CacheBackInserter backInserter();

    long getCurrentSize() const;
    void setCurrentSize(long currentSize);

    State getState() const;
    void setState(State state);

    long getLastUse() const;
    void setLastUse(long lastUse);

    friend class CacheBackInserter;

private:
    std::vector<char> data;
    State state;
    long currentSize;
    long *cacheSize;
    long lastUse;
    std::vector<Client *> clients;
};

#endif //PROXY_CACHEENTRY_H
