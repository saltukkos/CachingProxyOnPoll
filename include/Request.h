//
// Created by saltukkos on 14.11.16.
//

#ifndef PROXY_REQUEST_H
#define PROXY_REQUEST_H

#include <string>
#include <queue>

class Request {
private:
    const std::vector<char> requestEnd = {'\r', '\n', '\r', '\n'};

    bool ready;
    std::vector<char> data;
    std::string host;
    std::string request;

    bool isEnd();
    void parseRequest();
public:
    Request();
    bool putData(const char *buff, size_t from, size_t len);
    const std::vector<char> &getData() const;
    std::string getHost() const;
    std::string getRequest() const;
};


#endif //PROXY_REQUEST_H
