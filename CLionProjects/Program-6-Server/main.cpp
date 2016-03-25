#include <iostream>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "ConnectionHandler.h"

using namespace std;

int main(int argc, char *argv[]) {
    int socketFileDescriptor;
    int connectedClientFileDescriptor;
    int portNumber;
    socklen_t ClientAddressSize;
    struct sockaddr_in server_address, client_address; //sockaddr_in is a structure containing an internet address. This structure is defined in netinet/in.h
    if(argc < 2){
        std::cout << "No Port Number provided as argument, attempting to use 5234 as default" << std::endl;
        portNumber = 5234;
    }else{
        std::cout << "Attempting to use " << argv[1] << " as the port to listen on" << std::endl;
        portNumber = atoi(argv[1]);
    }
    //AF_INET - Listen for internet connection(As opposed to local machine
    //SOCK_STREAM - Stream data rather then sending in chunks
    //0 - Use most appropriate connection method
    socketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if(socketFileDescriptor<0){
        std::cout << "Error opening Socket" << std::endl;
    }


    bzero((char *) &server_address, sizeof(server_address)); //Sets all values of server_address to zero to prepare for listener
    server_address.sin_family = AF_INET; //Server Address is listen to internet connections
    server_address.sin_port = htons(portNumber); //Port number to listen to. htons() converts port number to network byte order (Because computer networks are big endian)
    server_address.sin_addr.s_addr = INADDR_ANY; //Sets server address to its own ip address. Allows connections.

    //bind accepts 3 inputs
    //1 - the socket file descriptor
    //2 - the server address information (must be type casted as a sockaddr as shown below
    //3 - the size of the server address information


    if(bind(socketFileDescriptor, (struct sockaddr *) &server_address, sizeof(server_address)) < 0){
        std::cout << "ERROR on binding to socket" <<std::endl;
    }

    listen(socketFileDescriptor, 5); //Tells socket it will be listening for connection with at most 5 in the queue.

    ClientAddressSize = sizeof(client_address); //Connection made, get size data about client information variable to make accepting easier.

    //accept accepts 3 inputs. Accpet listens and waits for a connection. This is when the client will connect.
    //1 - the socket file descriptor of general socket connected to(connectedClientFileDescriptor represents the connection itself, socketFileDescriptor represents and unconnected listener)
    //2 - the client address information (must be type casted as a sockaddr as shown below
    //3 - the size of the client address information
    std::cout << "Listening for connections..." <<std::endl;
    connectedClientFileDescriptor = accept(socketFileDescriptor, (struct sockaddr *) &client_address, &ClientAddressSize);
    std::cout << "Client Attempting to connect..." <<std::endl;
    if(connectedClientFileDescriptor<0){
        std::cout << "Error on connection accept" << std::endl;
    } else {
        std::cout << "Client Connected! Spawning thread to handle client..." <<std::endl;
    }

    //Connection in connectedClientFileDescriptor, need to create class instance and spawn thread for managing client connection
    //For now this code allows one connection as a test.
    ConnectionHandler client1(connectedClientFileDescriptor);
}