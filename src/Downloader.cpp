//
// Created by saltuk on 12/9/16.
//

#include "Downloader.h"
#include "Proxy.h"
#include "CacheManager.h"
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <unistd.h>

Downloader::Downloader(const Request request, CacheEntryPtr entry) :
    cache(entry),
    request(request),
    requestPosition(0){
#ifdef DEBUG
    std::cerr << "started downloader for " <<
              request.getHost() << " : " <<
              request.getRequest() << std::endl;
#endif
    fd = socket(AF_INET, SOCK_STREAM|SOCK_NONBLOCK, 0);
    ops = POLLOUT;

    hostent *serverHost = gethostbyname(request.getHost().c_str());
    if (!serverHost)
        throw std::invalid_argument("unable to resolve host");

    sockaddr_in	servAddr = {0};
    servAddr.sin_family = AF_INET;
    memcpy(&servAddr.sin_addr.s_addr, serverHost->h_addr_list[0], sizeof(in_addr));
    servAddr.sin_port=htons(80);

    if (connect(fd, (sockaddr*)&servAddr, sizeof(sockaddr_in)) < 0 && errno != EINPROGRESS){
        perror("connecting");
        throw std::invalid_argument("unable to connect to host");
    }

    buf = new char[bufSize];
    entry->setState(CacheEntry::Downloading);
}

Downloader::~Downloader() {
#ifdef DEBUG
    std::cerr << "downloader for "
              << request.getHost() << " : "
              << request.getRequest()
              << "deleted" << std::endl;
#endif
    cache->setState(CacheEntry::Finished);
    delete[] (buf);
    buf = nullptr;
    close(fd);
}

bool Downloader::handlePoll(int op, PollStoragePtr storage, CacheManager &cacheManager) {
    if (op & (POLLHUP|POLLERR|POLLNVAL)) {
        storage->removePollable(this);
        return true;
    }
    if (op == POLLOUT){
        long least = request.getData().size() - requestPosition;
        long ret = write(fd, request.getData().data() + requestPosition, (size_t)least);
        if (ret < 0){
            ops = 0;
            cache->notifyClients();
            storage->removePollable(this);
            cache->setState(CacheEntry::Finished);
            cacheManager.detachEntry(request.getHost()+request.getRequest());
            return true;
        }

        requestPosition += ret;
        if (requestPosition == request.getData().size())
            ops = POLLIN;
        return false;
    }
    if (op == POLLIN){
        ssize_t len = read(fd, buf, bufSize);
        if (len <= 0){
            ops = 0;
            cache->notifyClients();
            cache->setState(CacheEntry::Finished);
            if (len < 0) {
                cacheManager.detachEntry(request.getHost()+request.getRequest());
            }
#ifdef DEBUG
            std::cerr << "downloading of "
                      << request.getHost() << " : "
                      << request.getRequest()
                      << (len < 0 ? "aborted" : "completed") << std::endl;
#endif
            storage->removePollable(this);
            return true;
        }
#ifdef DEBUG
        std::cerr << "downloading of "
                  << request.getHost() << " : "
                  << request.getRequest()
                  << "read " << len << "bytes" << std::endl;
#endif
        std::copy(buf, buf+len, cache->backInserter());

        long was = cache->getCurrentSize();
        cache->setCurrentSize(was + len);
        cache->notifyClients();

        static const int min_n = 11;
        if (was < min_n && was + len >= min_n){
            const char *p = &cache->getData()[0];
            int i;
            for(i = 0; i < min_n - 1 && *p != ' '; ++i, ++p);
            ++p;
            if (i < min_n - 1 && *p != '2')
                cacheManager.detachEntry(request.getHost()+request.getRequest());
        }

        return false;
    }
}

int Downloader::getFileDescriptor() const {
    return fd;
}

short Downloader::getOperations() const {
    return ops;
}
