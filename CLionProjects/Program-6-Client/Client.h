//
// Created by jeffrey on 3/31/16.
//

#ifndef PROGRAM_6_CLIENT_H
#define PROGRAM_6_CLIENT_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <functional>

using namespace std;

std::string readConnection();
int writeConnection(std::string dataToWrite);
hash<std::string> str_hash;
void mainClientHandler();
void userLogin();
void newUser();
void option1Handler();
void option2Handler();
void option3Handler();
void mainHandler();

int socketFileDescriptor; //File discription for socket used to make connection
char buffer[256]; //Buffer to store data for receive
bool authenticated = false;

#endif //PROGRAM_6_CLIENT_CLIENT_H
