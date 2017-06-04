//
// Created by saltuk on 12/9/16.
//

#ifndef PROXY_POLLSTARAGE_H
#define PROXY_POLLSTARAGE_H

#include "Proxy.h"

#include <vector>
#include <poll.h>
#include <memory>

class PollStorage;

using PollablePtr = std::shared_ptr<class Pollable>;
using PollStoragePtr = std::shared_ptr<PollStorage>;

class PollStorage {
private:
    std::vector<pollfd> fds;
    std::vector<PollablePtr> pollables;

    void deleteByIdx(long idx);
public:
    void addPollable(Pollable *p);
    void removePollable(Pollable *p);
    void removePollable(int fd);

    const std::vector<PollablePtr> &getPollables() const;
    std::vector<pollfd> &getFds();

    PollStorage() = default;
    PollStorage(PollStorage&) = delete;
    PollStorage& operator=(PollStorage&) = delete;
};


#endif //PROXY_POLLSTARAGE_H
