#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

using namespace std;

std::string readConnection();
int writeConnection(std::string dataToWrite);

int socketFileDescriptor; //File discription for socket used to make connection
char buffer[256]; //Buffer to store data for receive

int main(int argc, char *argv[])
{
    int portNumber, numberOfCharacters;
    struct sockaddr_in server_address;
    struct hostent *server;


    if (argc < 3) {
        //Stuff for prompting ip

    }else{
        portNumber = atoi(argv[2]);
        server = gethostbyname(argv[1]);
    }

    //AF_INET - Listen for internet connection(As opposed to local machine
    //SOCK_STREAM - Stream data rather then sending in chunks
    //0 - Use most appropriate connection method

    socketFileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFileDescriptor < 0)
        cout << "ERROR opening socket" << endl;

    if (server == NULL) {
        cout << "ERROR, no such host" << endl;
        exit(0);
    }

    bzero((char *) &server_address, sizeof(server_address)); //Sets all values of server_address to zero to prepare for listener

    server_address.sin_family = AF_INET; //Server Address is listen to internet connections
    bcopy((char *)server->h_addr, (char *)&server_address.sin_addr.s_addr, server->h_length); //Have to use bcopy because server->h_addr is a character string
    server_address.sin_port = htons(portNumber); //Port number to listen to. htons() converts port number to network byte order (Because computer networks are big endian)

    //Make a connection to the server using the socket defined at socketFileDescriptor
    if (connect(socketFileDescriptor,(struct sockaddr *) &server_address,sizeof(server_address)) < 0) {
        cout << "ERROR connecting" << endl;
    }

    cout << "Please enter the message: ";

    string test;
    getline(stdin, test);
    writeConnection(test);

    test = readConnection();

    cout << endl << test << endl;

    close(socketFileDescriptor);
    return 0;
}


std::string readConnection(){
    ssize_t numOfCharRead;
    bzero(buffer,256); //Zero out the buffer
    numOfCharRead = read(socketFileDescriptor,buffer,255);
    if (numOfCharRead < 0){
        cout << "ERROR reading from socket" << endl;
        return "";
    }
    std::string returnString(buffer);
    return returnString;
}

int writeConnection(std::string dataToWrite){
    ssize_t numOfCharRead;
    numOfCharRead = write(socketFileDescriptor,dataToWrite.c_str(),dataToWrite.length());
    if (numOfCharRead < 0){
        cout << "ERROR writing to socket" << endl;
        return -1;
    }
    return 0;
}
