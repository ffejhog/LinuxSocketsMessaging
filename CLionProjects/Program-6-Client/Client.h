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

const unsigned int HASH_SEED = 546954645;
std::string readConnection();
int writeConnection(std::string dataToWrite);
unsigned int passHash(const char* s, unsigned int seed);
void mainClientHandler();
void userLogin();
void newUser();
void option1Handler();
void option2Handler();
void option3Handler();
void option4Handler();
void option5Handler();
void option6Handler();
void mainHandler();

int socketFileDescriptor; //File discription for socket used to make connection
char buffer[512]; //Buffer to store data for receive
bool authenticated = false;

#endif //PROGRAM_6_CLIENT_CLIENT_H
