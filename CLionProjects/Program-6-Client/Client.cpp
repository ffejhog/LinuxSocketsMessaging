#include "Client.h"

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
    if (socketFileDescriptor < 0) {
        cout << "ERROR opening socket" << endl;
        return 0;
    }

    if (server == NULL) {
        cout << "ERROR, no such host" << endl;
        return 0;

    }

    bzero((char *) &server_address, sizeof(server_address)); //Sets all values of server_address to zero to prepare for listener

    server_address.sin_family = AF_INET; //Server Address is listen to internet connections
    bcopy((char *)server->h_addr, (char *)&server_address.sin_addr.s_addr, server->h_length); //Have to use bcopy because server->h_addr is a character string
    server_address.sin_port = htons(portNumber); //Port number to listen to. htons() converts port number to network byte order (Because computer networks are big endian)

    //Make a connection to the server using the socket defined at socketFileDescriptor
    if (connect(socketFileDescriptor,(struct sockaddr *) &server_address,sizeof(server_address)) < 0) {
        cout << "ERROR connecting" << endl;
        return 0;
    }

    mainClientHandler(); //Control handed to main File handler(THis is separate for simplicity purposes)
    if(authenticated) {
        //authenticatedClientHandler();
    }
    close(socketFileDescriptor);
    return 0;
}


void mainClientHandler(){
    string userinput;

    do {
        cout << "Please Choose...  " << endl;
        cout << "1. Login" << endl;
        cout << "2. Create New User" << endl;
        cout << "3. Exit" << endl;

        getline(cin, userinput);
        int userinputnum = stoi(userinput);

        switch (userinputnum) {
            case 1:
                userLogin();
                break;
            case 2:
                newUser();
                break;
            case 3:
                writeConnection("3");
                return;
            default:
                cout << "That is not a valid option" << endl << endl;
        }
        }while(!authenticated);

	mainHandler();



}

void userLogin(){
    string userName;
    string password;

    writeConnection("1"); //Tell Server to prepare for login
    string serverResponse = readConnection(); //Retreive the servers response
    if(serverResponse == "1"){
        //server is good to receive
        cout << "Username: ";
        getline(cin, userName);
        cout << "Password: ";
        getline(cin, password);
        //Hash password here
        writeConnection(userName + "|" + password);
        serverResponse = readConnection();
        if(serverResponse == "1"){
            cout << "Success! Logging in User..." << endl;
            authenticated = true;
            return;
        }else{
            //User not valid
            cout << "Username or Password incorrect, please try login again" << endl;
            return;
        }
    }else{
        //Server bad to receive, error
        cout << "Error from server, failed to authenticate - ERROR CODE: 1" << endl;
        return;
    }

}

void newUser(){
    string userName;
    string password;

    writeConnection("2"); //Tell Server to prepare for add user
    string serverResponse = readConnection(); //Retreive the servers response
    if(serverResponse == "1"){
        //server is good to receive
        cout << "Username: ";
        getline(cin, userName);
        cout << "Password: ";
        getline(cin, password);
        //Hash password here
        writeConnection(userName + "|" + password);
        serverResponse = readConnection();
        if(serverResponse == "1"){
            cout << "Success! Please login with your new account..." << endl;

            return;
        }else{
            //User not valid
            cout << "ERROR: User not added" << endl;
            return;
        }
    }else{
        //Server bad to receive, error
        cout << "Error from server, failed to add user - ERROR CODE: 1" << endl;
        return;
    }
}

void  mainHandler(){
//TODO: THIS METHOD --JEFF
}

void option1Handler(){
//PUT OPTION 1 STUFF HERE

}

void option2Handler(){
//PUT OPTION 2 STUFF HERE
    //Receiving "1" from server
    string status = readConnection();

    if (status == "1")
    {
        cout << "Received...Returning a name...BOB " << endl;
        writeConnection("BOB");
    }
        cout << "ERROR! Did not receive from server" <<endl;
}

void option3Handler(){


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
