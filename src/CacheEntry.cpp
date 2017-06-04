//
// Created by saltuk on 11/25/16.
//

#include "CacheEntry.h"
#include "Client.h"
#include <algorithm>

CacheEntry::CacheEntry(long *cacheSize):
        state(Empty),
        currentSize(0),
        cacheSize(cacheSize),
        lastUse(0){}

CacheEntry::State CacheEntry::getState() const {
    return state;
}

void CacheEntry::setState(CacheEntry::State state) {
    CacheEntry::state = state;
}

long CacheEntry::getCurrentSize() const {
    return currentSize;
}

void CacheEntry::setCurrentSize(long currentSize) {
    CacheEntry::currentSize = currentSize;
}

const std::vector<char> &CacheEntry::getData() const{
    return data;
}

void CacheEntry::addClient(Client *client) {
    clients.push_back(client);
}

void CacheEntry::deleteClient(Client *client) {
    auto it = std::find(clients.begin(), clients.end(), client);
    if (it == clients.end())
        throw std::invalid_argument("there's no such client");

    clients.erase(it);
}

unsigned long CacheEntry::clientsCount() {
    return clients.size();
}

void CacheEntry::notifyClients() {
    for (auto& client: clients)
        client->notify();
}

CacheBackInserter CacheEntry::backInserter() {
    return (*this);
}

CacheEntry::~CacheEntry() {
    *cacheSize -= data.size();
}

long CacheEntry::getLastUse() const {
    return lastUse;
}

void CacheEntry::setLastUse(long lastUse) {
    CacheEntry::lastUse = lastUse;
}

CacheBackInserter::CacheBackInserter(CacheEntry &x) :
        vector(x.data),
        size(x.cacheSize) {}

CacheBackInserter &CacheBackInserter::operator=(const char &value) {
    vector.push_back(value);
    ++(*size);
    return *this;
}

CacheBackInserter CacheBackInserter::operator++(int) { return *this; }

CacheBackInserter &CacheBackInserter::operator++() { return *this; }

CacheBackInserter &CacheBackInserter::operator*() { return *this; }
