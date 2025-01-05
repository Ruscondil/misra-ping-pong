#include "sender.h"
#include <thread>
#include <chrono>

Sender::Sender(const std::string &address, int port)
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        std::cerr << "Error opening socket" << std::endl;
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, address.c_str(), &server_addr.sin_addr) <= 0)
    {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        exit(1);
    }

    while (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        std::cerr << "Connection Failed, retrying in 1 second..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void Sender::send(int64_t number)
{
    int64_t be_number = htobe64(number);
    if (write(sockfd, &be_number, sizeof(be_number)) < 0)
    {
        std::cerr << "Error sending data" << std::endl;
    }
}

Sender::~Sender()
{
    close(sockfd);
}
