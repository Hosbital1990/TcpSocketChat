#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>

#define PORT 8080
#define BUFFER_SIZE 1024

class Client {

public:

Client(std::string client_name , const char role);

void run();


~Client();

private:

/// @brief 
bool is_chat_ongoing = true;
std::string client_name;
const char role;

struct RegisterPacket
{

std::string clientName;
std::string clientAddress;
std::string clinetPort;

};

//Member function
void receivedMessageHandller(int client_fd );
void transmitMessageHandler(int client_fd);


};

#endif // CLIENT_H