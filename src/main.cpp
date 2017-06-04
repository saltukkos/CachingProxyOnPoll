#include <csignal>
#include "HTTPProxy.h"

bool working;

void onExit(int){
    working = false;
}

int main(int argc, char **argv){

    working = true;
    signal(SIGINT, onExit);
    signal(SIGPIPE, SIG_IGN);

    int port = 4243;

    if (argc > 1) {
        port = atoi(argv[1]);
    }

    HTTPProxy proxy(port);
    proxy.run();
}