//
// Created by saltuk on 12/7/16.
//

#include "Client.h"

#include "Downloader.h"
#include "Proxy.h"

Client::Client(int socket) :
    socket(socket),
    responsePosition(0),
    state(Client::WriteToServer),
    buffer(new char[1024*1024])
{
//    std::cerr << "create client with buffer " << (void*)buffer << this << std::endl;
#ifdef DEBUG
    std::cerr << "client(" << socket << ")"
              << " created" << std::endl;
#endif
}

Client::~Client() {
 //   std::cerr << "delete client with buffer " << (void*)buffer << this << std::endl;
#ifdef DEBUG
    std::cerr << "client(" << socket << ")"
              << "deleted" << std::endl;
#endif
    if (entry)
        entry->deleteClient(this);
    close(socket);
    delete [] buffer;
   // buffer = nullptr;
}

bool Client::handlePoll(int op, PollStoragePtr storage, CacheManager &cacheManager) {
    if (op & (POLLHUP|POLLERR|POLLNVAL)) {
        storage->removePollable(this);
        return true;
    }
    if (op & POLLIN){
        if (state != Client::WriteToServer)
            throw std::logic_error("Read from client not in WriteToServer state");
        auto size = read(socket, buffer, buff_size);
        if (size < 0){
            if (errno != EAGAIN){
                perror("read");
                storage->removePollable(this);
                return true;
            }
            return false;
        }
        if (size == 0 || request.putData(buffer, 0, (size_t)size)) {
            state = Client::ReadFromServer;
            entry = cacheManager.getEntry(request.getHost() + request.getRequest());
            entry->addClient(this);
            if (entry->getState() == CacheEntry::Empty){
                Downloader *downloader;
                try {
                    downloader = new Downloader(request, entry);
                } catch (std::exception e){
                    entry->setState(CacheEntry::Finished);
                    cacheManager.detachEntry(request.getHost()+request.getRequest());
#ifdef DEBUG
                    std::cerr << "unable to start downloader for "
                              << request.getHost() << " : "
                              << request.getRequest() << std::endl;
#endif
                    return false;
                }
                storage->addPollable(downloader);
            }
        }
        return false;
    }
    if (op & POLLOUT){
        if (state != Client::ReadFromServer)
            throw std::logic_error("Write to client not in ReadFromServer state");
        long least = entry->getCurrentSize() - responsePosition;
        assert(least >= 0);
        if (least == 0){
            if (entry->getState() == CacheEntry::Finished){
                storage->removePollable(this);
                state = Finish;
                return true;
            }
            else {
                state = Wait;
                return false;
            }
        }
        long ret = write(socket, entry->getData().data() + responsePosition, (size_t) least);
        if (ret < 0){
            perror("write");
            storage->removePollable(this);
            return true;
        }
        responsePosition += ret;
#ifdef DEBUG
        std::cerr << "client for"
                  << request.getHost() << " : "
                  << request.getRequest()
                  << "read " << responsePosition << std::endl;
#endif
        return false;
    }
}

int Client::getFileDescriptor() const {
    return socket;
}

void Client::notify() {
    state = ReadFromServer;
}

short Client::getOperations() const {
    if (state == WriteToServer)
        return POLLIN;
    if (state == ReadFromServer)
        return POLLOUT;
    return 0;
}
