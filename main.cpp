#include <iostream>
#include <string>
#include "sender.h"
#include "receiver.h"
#include "misra.h"

int main(int argc, char *argv[])
{
    if (argc != 5)
    {
        std::cerr << "Usage: " << argv[0] << " <address> <send_port> <receive_port> <starting>" << std::endl;
        return 1;
    }

    std::string address = argv[1];
    int send_port = std::stoi(argv[2]);
    int receive_port = std::stoi(argv[3]);
    bool starting = std::stoi(argv[4]);

    Receiver receiver(receive_port);
    Sender sender(address, send_port);
    Misra misra(sender);

    int client_socket = receiver.acceptConnection();

    if (starting)
    {
        std::cout << "Starting" << std::endl;
        sender.send(1);
        sender.send(-1);
    }

    while (true)
    {
        int64_t number = receiver.receive(client_socket);
        misra.process(number);
    }

    return 0;
}
