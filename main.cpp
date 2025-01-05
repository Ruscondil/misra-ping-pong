#include <iostream>
#include <string>
#include "sender.cpp"
#include "receiver.cpp"
#include "misra.cpp"

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        std::cerr << "Usage: " << argv[0] << " <address> <send_port> <receive_port>" << std::endl;
        return 1;
    }

    std::string address = argv[1];
    int send_port = std::stoi(argv[2]);
    int receive_port = std::stoi(argv[3]);

    Receiver receiver(receive_port);
    Sender sender(address, send_port);
    Misra misra(sender);

    int client_socket = receiver.acceptConnection();

    while (true)
    {
        int64_t number = receiver.receive(client_socket);
        std::cout << "Received number: " << number << std::endl;

        misra.process(number);
    }

    return 0;
}
