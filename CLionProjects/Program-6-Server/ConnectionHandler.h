//
// Created by jeffrey on 3/25/16.
//

#ifndef PROGRAM_6_SERVER_CONNECTIONHANDLER_H
#define PROGRAM_6_SERVER_CONNECTIONHANDLER_H

#include <iostream>
#include <string.h>
#include <unistd.h>

class ConnectionHandler {
    public:
        ConnectionHandler(int ConnectedClientFileDescriptor);
        void Test1Method();
        void error(const char *msg);
        void closeConnection();
        void mainHandler();
        void option1Handler();
        bool checkIfAuthenticated();

    private:
        int ClientFileDescriptor;
        ssize_t numOfCharRead;
        char buffer[256];
        bool authenticated;
        std::string userName;
        std::string readConnection();
        int writeConnection(std::string dataToWrite);
};


#endif //PROGRAM_6_SERVER_CONNECTIONHANDLER_H
