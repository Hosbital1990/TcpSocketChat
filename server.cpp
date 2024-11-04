#include <iostream>      // For std::cout
#include <cstring>      // For strlen
#include <unistd.h>     // For read(), close()
#include <sys/socket.h> // For socket(), send(), recv()
#include <netinet/in.h> // For sockaddr_in, INADDR_ANY
#include <arpa/inet.h>  // For inet_addr() and other network functions
#include <thread>
#include "server.h"


Server::Server()
{ }

void Server::run()
{

//create Socket

int server_fd , new_socket ;  // df : File Descriptor
server_fd= socket(AF_INET, SOCK_STREAM, 0);
if(!server_fd)
{
        perror("Socket failed");
        exit(-1);  // terminate the program
}

//Allocate Address

    struct sockaddr_in address; 

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Binding socket to the port
    if(bind(server_fd,(struct sockaddr*)&address, sizeof(address))<0)
    {
        perror("Listen failed");
        return;

    }

    //Listen for connection from client
    if (listen(server_fd,10) < 0)
    {
            perror("Listen failed");    
            exit(-1);
    }

    std::cout << "Server listening on port " << PORT << std::endl;

    int addrlen = sizeof(address);
   while (true) {
    //std::this_thread::sleep_for(std::chrono::milliseconds(500));
        new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        if (new_socket < 0) {
            std::cerr << "Accept failed\n";
            continue;
        }

std::cout << "\033[33m          New connection Accepted \n\033[0m";
    // std::cout << "\033[33mThread ID: " << std::this_thread::get_id() << "\033[0m" << std::endl;

     std::jthread ([this, new_socket](){

        this->connectionHandler(new_socket);
    }).detach() ;

// std::cout << "\033[33mThread ID: " << std::this_thread::get_id() << "\033[0m" << std::endl;
// std::cout << "\033[31m  New Thread Created!?@#$%^ \n\033[0m";

    //std::this_thread::sleep_for(std::chrono::milliseconds(100));

   }

    close(server_fd);

}

void Server::connectionHandler(int client_fd)
{

//First Step register new client
char message[BUFFER_SIZE] = {0};
RegisterPacket register_packet;

    //Wait for Register packet
    int bytesReceived = recv(client_fd,&message,BUFFER_SIZE ,0 );

    if ( bytesReceived  < 0)
    {
        /* code */
        perror("Something wrong with read registeration packer");
        return ;  // Just for leave the thread to kill it
    }

    std::cout << "Here is message from client: "<< message << " \n"  ;

    register_packet.clientName = std::string(message, MAX_SIZE_OF_CLIENT_ID);
    register_packet.clientAddress = std::string(message + MAX_SIZE_OF_CLIENT_ID, SIZE_OF_CLIENT_ADDRESS);
    register_packet.clinetPort = std::string(message + MAX_SIZE_OF_CLIENT_ID + SIZE_OF_CLIENT_ADDRESS, SIZE_OF_CLIENT_PORT);

    std::cout << "Size of: " << register_packet.clientName.size() << "  "  
                             << register_packet.clientAddress.size() <<  "  "  
                             << register_packet.clinetPort.size() << " "
                             << sizeof(register_packet) << std::endl;

    connectedClients[client_fd] = register_packet;  //Store new connected client

    std::cout << "Client ID " << register_packet.clientName <<" Registered!" << std::endl ;

    //waiting for new chat demand with specified client with current client (via server)
    while (true)
    {

    /* code */
    bytesReceived = recv(client_fd,&message,BUFFER_SIZE ,0 );
    if ( bytesReceived  < 0)
    {
    /* code */
        perror("Something wrong with read registeration packer");
        close(client_fd);
        connectedClients.erase(client_fd);
        break;
    }

    MessagePacket message_packet ;
    message_packet.recipientID =     std::string(message , MAX_SIZE_OF_CLIENT_ID );
    message_packet.messageContent =  std::string(message + MAX_SIZE_OF_CLIENT_ID, bytesReceived - MAX_SIZE_OF_CLIENT_ID );

// if (bytesReceived >= MAX_SIZE_OF_CLIENT_ID) {
//     message_packet.recipientID = std::string(message, MAX_SIZE_OF_CLIENT_ID);

//     if (bytesReceived > MAX_SIZE_OF_CLIENT_ID) {
//         message_packet.messageContent = std::string(
//             message + MAX_SIZE_OF_CLIENT_ID,
//             bytesReceived - MAX_SIZE_OF_CLIENT_ID
//         );
//     } else {
//         message_packet.messageContent = "";  // No content provided
//     }
// } else {
//     std::cerr << "Received packet is too small to contain a valid recipient ID" << std::endl;
//     return;  // Handle this error as needed
// }




    for (const auto& [key, val]: connectedClients){
        if(message_packet.recipientID == val.clientName)
        {
send(key, message_packet.messageContent.c_str(), message_packet.messageContent.size(), MSG_NOSIGNAL);
    
        break;
        }
      }

    }
   close(client_fd);
}

Server::~Server()
{}