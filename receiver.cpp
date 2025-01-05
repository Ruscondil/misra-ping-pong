#include "receiver.h"

Receiver::Receiver(int port)
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        std::cerr << "Error opening socket" << std::endl;
        exit(1);
    }

    int opt = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        std::cerr << "setsockopt failed" << std::endl;
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        std::cerr << "Bind failed" << std::endl;
        exit(1);
    }

    if (listen(sockfd, 3) < 0)
    {
        std::cerr << "Listen failed" << std::endl;
        exit(1);
    }
}

int Receiver::acceptConnection()
{
    int new_socket;
    socklen_t addrlen = sizeof(server_addr);
    if ((new_socket = accept(sockfd, (struct sockaddr *)&server_addr, &addrlen)) < 0)
    {
        std::cerr << "Accept failed" << std::endl;
        exit(1);
    }
    return new_socket;
}

int64_t Receiver::receive(int client_socket)
{
    int64_t be_number;
    if (read(client_socket, &be_number, sizeof(be_number)) < 0)
    {
        std::cerr << "Error receiving data" << std::endl;
        exit(1);
    }
    return be64toh(be_number);
}

Receiver::~Receiver()
{
    close(sockfd);
}
