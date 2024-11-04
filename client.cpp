#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <thread>
#include "client.h"


Client::Client(std::string client_name, const char role)
: role(role), client_name(client_name)
{
    is_chat_ongoing =true;
}

void Client::run()
{
    int client_fd =0 ;
    struct sockaddr_in serv_addr;

    //Socket creation
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(client_fd <0)
    {
        perror("Socket creation error");
        exit(-1);
    }

//Define server address
    serv_addr.sin_family = AF_INET;
serv_addr.sin_port =    htons(PORT);
    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(-1);
    }


    // Connect to the server
    if (connect(client_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        exit(-1);
    }
    else 
    {

        std::cout << " client " <<  client_name << "  connected\n" ;
    }

    // Transmit registeration packet
    struct RegisterPacket register_packet;
    register_packet.clientName = "Hosbital0"+client_name;
    register_packet.clientAddress= "192.168.13.1";
    register_packet.clinetPort = "8080";

    // std::cout << "Size of: " << register_packet.clientName.size() << "  "  
    //                          << register_packet.clientAddress.size() <<  "  "  
    //                          << register_packet.clinetPort.size() << " "
    //                          << sizeof(register_packet) << std::endl;

    std::string message = register_packet.clientName+register_packet.clientAddress+register_packet.clinetPort;
    int numberSent = send(client_fd, message.c_str(), message.size(), 0) ;
    if(  numberSent  >0){
    //send(client_fd, &message, sizeof(message), 0);
   // std::cout << "Register Packet sent to server!!!! " << "Number of Sent bytes: " << numberSent << std::endl;

    }
    else {
   // std::cout << "Something went wrong with packet sending." << std::endl;        
    }

switch (role)
{
case 'T':
    /* code */
std::jthread([this,client_fd](){

this->transmitMessageHandler(client_fd);

}).detach();
    break;

case 'R':
std::jthread([this,client_fd](){

this->receivedMessageHandller(client_fd);

}).detach();
    break;

default:


    break;
}

}

void Client::receivedMessageHandller(int client_fd )
{


while (true)
{
    /* code */
        char buffer[BUFFER_SIZE] = {0};
    // Read server response
    int valread = recv(client_fd, buffer, BUFFER_SIZE,0);
    if (valread > 0) {
       // std::cout << buffer << std::endl;
        std::cout << ">>>>>>> " << std::string(buffer, valread) << std::endl;

    }
}

if(client_fd>0)
{
    close(client_fd);

}
}

void Client::transmitMessageHandler(int client_fd)
{

while(true)
{
std::string chatContent;
std::cout << "<<<<<<< " ;
std::getline(std::cin, chatContent);
chatContent = "Hosbital02"+chatContent;
if(send(client_fd, chatContent.c_str(), chatContent.size(),0) < 0)
{
perror("Send chatContent Failed");
exit(-1);
}
else 
{

std::cout << "Successfully sent! \n" ;

}
std::this_thread::sleep_for(std::chrono::milliseconds(100));
}
close(client_fd);

}


Client::~Client()
{}