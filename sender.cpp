#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

class Sender
{
public:
    Sender(const std::string &address, int port)
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

        if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        {
            std::cerr << "Connection Failed" << std::endl;
            exit(1);
        }
    }

    void send(int64_t number)
    {
        int64_t be_number = htobe64(number);
        if (write(sockfd, &be_number, sizeof(be_number)) < 0)
        {
            std::cerr << "Error sending data" << std::endl;
        }
    }

    ~Sender()
    {
        close(sockfd);
    }

private:
    int sockfd;
    struct sockaddr_in server_addr;
};
