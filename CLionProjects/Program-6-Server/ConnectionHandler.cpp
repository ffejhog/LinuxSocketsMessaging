//
// Created by jeffrey on 3/25/16.
//

#include "ConnectionHandler.h"


ConnectionHandler::ConnectionHandler(int ConnectedClientFileDescriptor){
    ClientFileDescriptor = ConnectedClientFileDescriptor;
    bzero(buffer,256);

}

void ConnectionHandler::Test1Method(){
    numOfCharRead = read(ClientFileDescriptor,buffer,255);
    if (numOfCharRead < 0){
        error("ERROR reading from socket");
    }
    printf("Here is the message: %s\n",buffer);
    numOfCharRead = write(ClientFileDescriptor,"I got your message",18);
    if (numOfCharRead < 0){
        error("ERROR writing to socket");
    }



}

void ConnectionHandler::error(const char *msg){
    std::cout << &msg << std::endl;

}


void ConnectionHandler::closeConnection() {
    close(ClientFileDescriptor);
}