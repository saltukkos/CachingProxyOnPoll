//
// Created by saltuk on 12/9/16.
//

#include "Acceptor.h"
#include "Client.h"
#include "CacheManager.h"
#include <sys/socket.h>
#include <arpa/inet.h>

Acceptor::Acceptor(int port) {
    sockaddr_in serv_addr = {0};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons((uint16_t)port);

    fd = socket(AF_INET, SOCK_STREAM|SOCK_NONBLOCK, 0);
    if (bind(fd, (sockaddr*)&serv_addr, sizeof(serv_addr)) < 0){
        perror("bind");
        throw std::invalid_argument("unable to bind on this address");
    }

    listen(fd, 50);
}

bool Acceptor::handlePoll(int op, PollStoragePtr storage, CacheManager &cacheManager) {
    if (op & (POLLHUP|POLLERR|POLLNVAL)) {
        storage->removePollable(this);
        return true;
    }
    if(op == POLLIN) {
        Client *client = new Client(accept4(fd, (struct sockaddr *) NULL, NULL, SOCK_NONBLOCK));
        storage->addPollable(client);
        return false;
    }
}

int Acceptor::getFileDescriptor() const{
    return fd;
}

Acceptor::~Acceptor() {
    close(fd);
}

short Acceptor::getOperations() const {
    return POLLIN;
}
