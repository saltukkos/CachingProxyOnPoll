//
// Created by saltuk on 12/9/16.
//

#include "PollStorage.h"
#include "Pollable.h"
#include <algorithm>

void PollStorage::addPollable(Pollable *p) {
    assert(fds.size() == pollables.size());

    pollfd pollfd = {.fd = p->getFileDescriptor(), .events = p->getOperations()};

    fds.push_back(pollfd);
    pollables.push_back(std::shared_ptr<Pollable>(p));
}

void PollStorage::removePollable(Pollable *p) {
    assert(fds.size() == pollables.size());
    auto it = std::find_if(pollables.begin(), pollables.end(),
    [p](const PollablePtr &ptr){ return p == ptr.get();});

    long idx = std::distance(pollables.begin(), it);
    deleteByIdx(idx);
}

void PollStorage::removePollable(int fd) {
    assert(fds.size() == pollables.size());
    auto it = std::find_if(fds.begin(), fds.end(),
                           [fd](const pollfd &pfd){return pfd.fd == fd;});

    long idx = std::distance(fds.begin(), it);
    deleteByIdx(idx);
}

void PollStorage::deleteByIdx(long idx) {
    if (idx >= fds.size())
        return;
    assert(fds[idx].fd == pollables[idx]->getFileDescriptor());

    fds.erase(fds.begin() + idx);
    pollables.erase(pollables.begin() + idx);
}

const std::vector<PollablePtr> &PollStorage::getPollables() const{
    return pollables;
}

std::vector<pollfd> &PollStorage::getFds() {
    return fds;
}
