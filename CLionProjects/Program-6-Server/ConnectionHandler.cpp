//
// Created by jeffrey on 3/25/16.
//

#include "ConnectionHandler.h"


ConnectionHandler::ConnectionHandler(int ConnectedClientFileDescriptor){
    ClientFileDescriptor = ConnectedClientFileDescriptor;


    int numOfAuthTries = 0;

    //Client auth here before being passed to mainHandler
    while(numOfAuthTries <3) {
        std::string enteredUserName = readConnection();

        std::string enteredPassword = readConnection();
        if (enteredUserName != "" && enteredPassword != "") {
            //check username and pass against all passwords

            //if they arent in database
            //then increment numOfAuthTries by one, otherwise success, set username to the entered username and set Authenticated to true. Also be sure to send success to client
            //Also need to remember to send response to client if they fail all 3 tries
        } else {
            std::cout << "Error on auth read" << std::endl;
            closeConnection();
        }
    }



}

bool ConnectionHandler::checkIfAuthenticated(){
    return authenticated;
}

void ConnectionHandler::mainHandler() {
    int enteredCommand = atoi(readConnection().c_str());
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
