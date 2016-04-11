//
// Created by jeffrey on 3/25/16.
//

#ifndef PROGRAM_6_SERVER_CONNECTIONHANDLER_H
#define PROGRAM_6_SERVER_CONNECTIONHANDLER_H

#include <iostream>
#include <string.h>
#include <unistd.h>
#include <fstream>

using namespace std;

class ConnectionHandler {
    public:
        ConnectionHandler(int ConnectedClientFileDescriptor);
        void Test1Method();
        void error(const char *msg);
        void closeConnection();
        void mainHandler();
        void option1Handler();
        void option2Handler();
        void option3Handler();
        void option4Handler();
        void option5Handler();
        void option6Handler();
        void loginHandler();
        void newUser();
        bool checkIfAuthenticated();



    private:
        int ClientFileDescriptor;
        ssize_t numOfCharRead;
        char buffer[512];
        bool authenticated;
        std::string userName;
        std::string readConnection();
        int writeConnection(std::string dataToWrite);

        const string FILE_NAME_DIR = "/home/jeffrey/Documents/program-6/CLionProjects/Program-6-Server/data/";
};


#endif //PROGRAM_6_SERVER_CONNECTIONHANDLER_H
