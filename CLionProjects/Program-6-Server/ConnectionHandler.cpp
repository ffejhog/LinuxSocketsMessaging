//
// Created by jeffrey on 3/25/16.
//

#include "ConnectionHandler.h"


ConnectionHandler::ConnectionHandler(int ConnectedClientFileDescriptor){
    ClientFileDescriptor = ConnectedClientFileDescriptor;

    std::string enteredCommand = readConnection();
    int enteredCommandnum = stoi(enteredCommand);
   switch(enteredCommandnum) {
       case 1:
           loginHandler();
           break;
       case 2:
           //newUser();
       default:
           return;
   }

}

void ConnectionHandler::loginHandler(){
    writeConnection("1"); // Letting client know Server is ready to recieve

    std::string enteredUserName = readConnection();
    std::string enteredPassword = readConnection();

    //Check if valid stuff here

    writeConnection("1"); //User is authenticated, let client know
    authenticated = true;
}






bool ConnectionHandler::checkIfAuthenticated(){
    return authenticated;
}

void ConnectionHandler::mainHandler() {
    int enteredCommand = stoi(readConnection());
    if(enteredCommand == 0){ //Check for success
        std::cout << "ERROR on client read" <<std::endl;
        closeConnection();
    }


    bool terminateConnection = false;

    while(!terminateConnection){

        switch(enteredCommand){ //Handler for each of the 7 commands. If case is to quit then set terminateConnection to true

        }

    }

}

void ConnectionHandler::option1Handler() {


}




//TEST METHOD FOR TEST CLIENT

void ConnectionHandler::Test1Method(){
    std::cout << "Here is the message: " << readConnection() << std::endl;
    writeConnection("I got your message");
}

void ConnectionHandler::error(const char *msg){
    std::cout << &msg << std::endl;

}


void ConnectionHandler::closeConnection() {
    close(ClientFileDescriptor);
}

std::string ConnectionHandler::readConnection(){
    bzero(buffer,256);
    numOfCharRead = read(ClientFileDescriptor,buffer,255);
    if (numOfCharRead < 0){
        error("ERROR reading from socket");
        return "";
    }
    std::string returnString(buffer);
    return returnString;
}

int ConnectionHandler::writeConnection(std::string dataToWrite){
    numOfCharRead = write(ClientFileDescriptor,dataToWrite.c_str(),dataToWrite.length());
    if (numOfCharRead < 0){
        error("ERROR writing to socket");
        return -1;
    }
    return 0;
}
