//
// Created by saltukkos on 14.11.16.
//

#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <cstring>
#include <sstream>
#include "Request.h"

void pushStringToVector(std::string str, std::vector<char> &vector){
    for (char c : str){
        vector.push_back(c);
    }
    vector.push_back('\n');
}

void Request::parseRequest() {
    data.push_back(0);
    std::istringstream resp(data.data());
    std::string temp;
    std::vector<char> parsedRequest;
    int n = 0;
    while (std::getline(resp, temp) && temp != "\r"){
        if (n++ == 0){
            if (temp.substr(0, 3) != "GET") {
                throw std::invalid_argument("only GET method is supported");
            }

            ulong pos = temp.find_first_of(" ");
            if (temp.substr(pos + 1, 7) != "http://")
                throw std::invalid_argument("only HTTP protocol is supported");
            temp.erase(pos+1, 7);

            ulong pos2 = temp.find_first_of("/");
            host = temp.substr(pos+1, pos2 - pos -1);
            temp.erase(pos + 1, pos2 - pos -1);

            pos = temp.find_last_of("HTTP/");
            temp.replace(pos - 4, 8, "HTTP/1.0");
            request = temp;

        }
        else if (temp.find("Connection:") == 0){
            pushStringToVector("Connection: close\r\n", parsedRequest);
            continue;
        }
        pushStringToVector(temp, parsedRequest);

    }

    parsedRequest.push_back('\r');
    parsedRequest.push_back('\n');

    data = parsedRequest;
}

std::string Request::getHost() const {
    if (!ready)
        throw std::logic_error("request isn't ready yet");
    return host;
}

std::string Request::getRequest() const {
    if (!ready)
        throw std::logic_error("request isn't ready yet");
    return request;
}

bool Request::isEnd() {
    if (std::search(data.begin(), data.end(), requestEnd.begin(), requestEnd.end()) != data.end()) {
        if (!ready)
            parseRequest();
        ready = true;
    }
    return ready;
}

bool Request::putData(const char *buff, size_t from, size_t len) {
//    std::cerr << "read from " << (void*)buff << "by idx: " << from << " " << from+len-1 <<std::endl;
    for (size_t i = from; i < from + len; ++i){
        data.push_back(buff[i]);
    }

    return isEnd();
}

const std::vector<char> &Request::getData() const {
    return data;
}

Request::Request() :
        ready(false) {}
