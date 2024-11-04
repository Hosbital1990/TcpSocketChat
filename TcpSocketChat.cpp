#include <iostream>
#include <thread>
#include <memory>
#include <vector>
#include <any>
#include <queue>
#include <map>
#include <iostream>
#include <memory>
#include <mutex>
#include <condition_variable>

#include "server.h"
#include "client.h"

int main(int argc, char const *argv[])
{
    /* code */

    // Start the server in a jthread
    std::jthread serverThread([](){
        Server server;  // Assuming Server class has a run method or similar for operation
        server.run();
       // server.run();   // You should define what the server does in this method
    });


std::this_thread::sleep_for(std::chrono::milliseconds(100));

std::jthread clientThread02 ([](){  //Receiver

Client client("2", 'R'); 
client.run();

}) ;


// std::this_thread::sleep_for(std::chrono::milliseconds(2000));

// std::jthread clientThread01 ([](){

// Client client("1", 'T');  //Transmitter
// client.run();
// }) ;

    return 0;
}

// std::unique_lock<std::mutex> lock(mtx);