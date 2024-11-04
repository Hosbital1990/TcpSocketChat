#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <map>


#define PORT 8080
#define BUFFER_SIZE  1024

#define MAX_SIZE_OF_CLIENT_ID 10
#define SIZE_OF_CLIENT_ADDRESS 12
#define SIZE_OF_CLIENT_PORT 4

class Server 
{

public:

Server();
void run();

~Server();

private:

void connectionHandler(int client_fd);

struct RegisterPacket
{

std::string clientName;
std::string clientAddress;
std::string clinetPort;

};


struct MessagePacket
{
    /* data */
std::string recipientID ;  //Destination ID
std::string messageContent ;  


};


std::map<int , RegisterPacket> connectedClients ;

};

#endif  // SERVER_H