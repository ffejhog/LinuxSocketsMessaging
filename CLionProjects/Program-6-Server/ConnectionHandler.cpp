//
// Created by jeffrey on 3/25/16.
//

#include "ConnectionHandler.h"


ConnectionHandler::ConnectionHandler(int ConnectedClientFileDescriptor){
    ClientFileDescriptor = ConnectedClientFileDescriptor;


    do {
        std::string enteredCommand = readConnection();
        int enteredCommandnum = stoi(enteredCommand);
        switch (enteredCommandnum) {
            case 1:
                loginHandler();
                break;
            case 2:
                newUser();
                break;
            default:
                return;
        }
    }while(!authenticated);

}

void ConnectionHandler::loginHandler(){
    writeConnection("1"); // Letting client know Server is ready to recieve

    string enteredUserName = readConnection();

    fstream userDatabase;
    userDatabase.open("/home/jeffrey/Documents/program-6/CLionProjects/Program-6-Server/data/users.bin", ios::in);
    string readLine;

    if (userDatabase.is_open()){
        while ( getline (userDatabase,readLine) )
        {
            if(readLine == enteredUserName) {
                authenticated = true;
                break;
            }
        }
        userDatabase.close();
    }



    if(authenticated){
        writeConnection("1");//User is authenticated, let client know
    }else{
        writeConnection("0");//User is authenticated, let client know
    }
    //Check if valid stuff here



}

void ConnectionHandler::newUser(){
    writeConnection("1"); // Letting client know Server is ready to recieve

    string enteredUserName = readConnection();

    fstream userDatabase;
    userDatabase.open("/home/jeffrey/Documents/program-6/CLionProjects/Program-6-Server/data/users.bin", ios::out | ios::app);


    if (userDatabase.is_open()){
        userDatabase << enteredUserName;
    }

    writeConnection("1");//User is authenticated, let client know

}




bool ConnectionHandler::checkIfAuthenticated(){
    return authenticated;
}

void ConnectionHandler::mainHandler() {
    std::string enteredCommand = readConnection();
    int enteredCommandnum = std::stoi(enteredCommand);

    if(enteredCommandnum == 0){ //Check for success
        std::cout << "ERROR on client read" <<std::endl;
        closeConnection();
    }


    bool terminateConnection = false;


}

void ConnectionHandler::option1Handler() {
	
	// loads file with usernames and passwords, and creates a string to hold the current line of the text file
	fstream file("users.txt");
	String currentLine;
	
	// prints the title to the list of usernames
	cout << "List of Users:\n" << endl;
	
	// while loop loads each line of the file to currentLine one line at a time
	while(file >> currentLine) {
		// for loop checks each character for the '|" character which divides username from password in the file
		for(int i = 0; i < currentLine.length(); i++)
		{
			// once the '|' character is reached the for loop breaks
			if(currentLine.charAt(i).compare('|'))
				break;
			// the current character is printed which will form the complete current username when the loop is completed
			cout << currentLine.charAt(i);
		}
		// new line printed
		cout << endl;
	}

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
