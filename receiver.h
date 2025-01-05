#ifndef RECEIVER_H
#define RECEIVER_H

#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

class Receiver
{
public:
    Receiver(int port);
    int acceptConnection();
    int64_t receive(int client_socket);
    ~Receiver();

private:
    int sockfd;
    struct sockaddr_in server_addr;
};

#endif // RECEIVER_H
