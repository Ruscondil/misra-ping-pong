#ifndef SENDER_H
#define SENDER_H

#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

class Sender
{
public:
    Sender(const std::string &address, int port);
    void send(int64_t number);
    ~Sender();

private:
    int sockfd;
    struct sockaddr_in server_addr;
};

#endif // SENDER_H
