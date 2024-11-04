# this project provde a simple chat between to client via a server with c++ socket prgramming
# to do this you need to first define server address in client objects
# this is absolutelt matter if clients and server are in local or seperate network

# following find a simple client code to run in seperate machine to provide chat between to seperate machine (but still same network)
# g++-13 file.cpp -o runable  // to compile
# ./runable    // to run

#include <iostream>
#include <cstring>
#include <unistd.h>     // For close()
#include <sys/socket.h> // For socket(), send(), recv()
#include <netinet/in.h> // For sockaddr_in
#include <arpa/inet.h>  // For inet_addr()

#define PORT 8080
#define BUFFER_SIZE 1024

struct RegisterPacket
{

std::string clientName;
std::string clientAddress;
std::string clinetPort;

};


class Client {
public:
    Client(const std::string& serverAddress);
    void run();

private:
    int sock_fd;
    void transmitMessageHandler();
};

Client::Client(const std::string& serverAddress) {
    // Create socket
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Define server address
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, serverAddress.c_str(), &serverAddr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    // Connect to server
    if (connect(sock_fd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Connection to server failed");
        exit(EXIT_FAILURE);
    }
    // Transmit registeration packet
    struct RegisterPacket register_packet;
    register_packet.clientName = "Hosbital01";
    register_packet.clientAddress= "192.168.13.1";
    register_packet.clinetPort = "8080";

    // std::cout << "Size of: " << register_packet.clientName.size() << "  "  
    //                          << register_packet.clientAddress.size() <<  "  "  
    //                          << register_packet.clinetPort.size() << " "
    //                          << sizeof(register_packet) << std::endl;

    std::string message = register_packet.clientName+register_packet.clientAddress+register_packet.clinetPort;
    int numberSent = send(sock_fd, message.c_str(), message.size(), 0) ;
    if(  numberSent  >0){
    //send(client_fd, &message, sizeof(message), 0);
   // std::cout << "Register Packet sent to server!!!! " << "Number of Sent bytes: " << numberSent << std::endl;

    }
    else {
   // std::cout << "Something went wrong with packet sending." << std::endl;        
    }


}

void Client::run() {
    // Start transmitting messages
    transmitMessageHandler();
}

void Client::transmitMessageHandler() {
    while (true) {
        std::string chatContent;
        std::cout << "<<<<<<< ";
        std::getline(std::cin, chatContent);

        // Prepend recipient ID (e.g., "Hosbital02")
        chatContent = "Hosbital02:" + chatContent;

        // Send message to server
        if (send(sock_fd, chatContent.c_str(), chatContent.size(), 0) < 0) {
            perror("Send failed");
            break;
        } else {
            std::cout << "Successfully sent! \n";
        }

    }
    close(sock_fd);
}

int main() {
    std::string serverAddress = "192.168.1.3"; // Change this to your server IP if needed
    Client client(serverAddress);
    client.run();
    return 0;
}
