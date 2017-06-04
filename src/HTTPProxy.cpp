//
// Created by saltuk on 12/9/16.
//

#include "HTTPProxy.h"
#include "Proxy.h"
#include "Acceptor.h"

void HTTPProxy::run() {
    while (working){
        poll(storage->getFds().data(), storage->getFds().size(), -1);
        for (int i = 0; i < storage->getFds().size(); ++i){
            int rev;
            bool isDelete = false;

            try {
                if ((rev = storage->getFds()[i].revents) != 0) {
                    isDelete = storage->getPollables()[i]->handlePoll(rev, storage, cacheManager);
                }
            }
            catch (std::exception e){
                storage->removePollable(storage->getFds()[i].fd);
                isDelete = true;
            }
            if (isDelete)
                --i;
            else
                storage->getFds()[i].events = storage->getPollables()[i]->getOperations();
        }
    }
}

HTTPProxy::HTTPProxy(int port):
    storage(new PollStorage()) {
    storage->addPollable(new Acceptor(port));
}
