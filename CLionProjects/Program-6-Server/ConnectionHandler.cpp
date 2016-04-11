//
// Created by jeffrey on 3/25/16.
//

#include "ConnectionHandler.h"


ConnectionHandler::ConnectionHandler(int ConnectedClientFileDescriptor){
    ClientFileDescriptor = ConnectedClientFileDescriptor;


    do {
        std::string enteredCommand = readConnection();
        if(enteredCommand==""){
            return;
        }
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
    userDatabase.open(FILE_NAME_DIR+"users.bin", ios::in);
    string readLine;


    if (userDatabase.is_open()){
        while ( getline (userDatabase,readLine) )
        {
            if(readLine == enteredUserName) {
                authenticated = true;

                for(int i = 0; i < enteredUserName.length(); i++)
                {
                    // once the '|' character is reached the for loop breaks
                    if(enteredUserName.at(i) == '|')
                        break;
                    // the current character is added to the String for each a username
                    userName += enteredUserName.at(i);
                }

                break;
            }
        }


    }
    userDatabase.flush();
    userDatabase.close();

    if(authenticated){
        writeConnection("1");//User is authenticated, let client know
    }else{
        writeConnection("0");//User is authenticated, let client know
    }
    //Check if valid stuff here



}

void ConnectionHandler::newUser() {
    writeConnection("1"); // Letting client know Server is ready to recieve

    string enteredUserNamePassword = readConnection();

    fstream userDatabase;
    userDatabase.open(FILE_NAME_DIR + "users.bin", ios::in | ios::out | ios::app);


    string enteredUserName;
    for (int i = 0; i < enteredUserNamePassword.length(); i++) {
        // once the '|' character is reached the for loop breaks
        if (enteredUserNamePassword.at(i) == '|')
            break;
        // the current character is added to the String of usernames that will be written to the client
        enteredUserName += enteredUserNamePassword.at(i);
    }

    string currentString, currentLine;
    if (userDatabase.is_open() && userDatabase.good()) {
        while (getline(userDatabase, currentLine)) {

            // for loop checks each character for the '|' character which divides username from password in the file
            for (int i = 0; i < currentLine.length(); i++) {
                // once the '|' character is reached the for loop breaks
                if (currentLine.at(i) == '|')
                    break;
                // the current character is added to the String of usernames that will be written to the client
                currentString += currentLine.at(i);
            }
            // a comma added to sepereate usernames in the String
            if (currentString == enteredUserName) {
                writeConnection("0");
                break;
            } else {
                userDatabase << enteredUserNamePassword << endl;
                writeConnection("1");
                break;
            }
        }
        userDatabase.close();

    }
}



bool ConnectionHandler::checkIfAuthenticated(){
    return authenticated;
}

void ConnectionHandler::mainHandler() {
        std::string enteredCommand;
        while (1){
            enteredCommand = "0";
            enteredCommand = readConnection();
            if(enteredCommand==""){
                return;
            }
            int enteredCommandnum = std::stoi(enteredCommand);

            if(enteredCommandnum == 0){ //Check for success
                std::cout << "ERROR on client read" <<std::endl;
                closeConnection();
	            break;
            }else if(enteredCommandnum == 1){
			    option1Handler();
            }else if(enteredCommandnum == 2){
			    option2Handler();
            }else if(enteredCommandnum == 3){
                option3Handler();
            }else if(enteredCommandnum == 4){
                option4Handler();
            }else if(enteredCommandnum == 5){
                option5Handler();
            }else if(enteredCommandnum == 6){
                option6Handler();
		    }else if(enteredCommandnum == 7){
                return;
            }
    }




}

void ConnectionHandler::option1Handler() {

    fstream usersFile;
    usersFile.open(FILE_NAME_DIR+"users.bin", ios::in);
    string currentLine, currentString;

    // while loop loads each line of the file to currentLine one line at a time
    if(usersFile.is_open() && usersFile.good()) {
        while ( getline (usersFile,currentLine) ) {

            // for loop checks each character for the '|' character which divides username from password in the file
            for (int i = 0; i < currentLine.length(); i++) {
                // once the '|' character is reached the for loop breaks
                if (currentLine.at(i) == '|')
                    break;
                // the current character is added to the String of usernames that will be written to the client
                currentString += currentLine.at(i);
            }
            // a comma added to sepereate usernames in the String
            currentString += ",";
        }
        // the String is terminated with a '|' character
        currentString += "|";

        cout << currentString << endl;
        // the String of usernames is written to the client via the writeConnection function
        writeConnection(currentString);
    }

}

void ConnectionHandler::option2Handler() {

	// sends a 1 to the client to indicate the server is ready recieve a String
	writeConnection("1");

	// stores the String from the client to newPartner
	string newPartner = readConnection();

	// opens/loads the users.txt file
	fstream usersFile(FILE_NAME_DIR+"users.bin", ios::in);
	// creates Strings representing the currentLine and each user from the database file
	string currentLine, currentString;
	// bool represents if there's a match in the user.txt file
	bool userExists = false;

	// for loop checks each character for the '|' character which divides username from password in the file
	while(getline(usersFile, currentLine) )	{

		// creates an empty String for each line
		currentString = "";

		// for loop checks each character for the '|' character which divides username from password in the file
		for(int i = 0; i < currentLine.length(); i++)
		{
			// once the '|' character is reached the for loop breaks
			if(currentLine.at(i) == '|')
				break;
			// the current character is added to the String for each a username
			currentString += currentLine.at(i);
		}

		// conditional compares the current username to the partner from the client
		if(newPartner == currentString) {
			// bool assigned the value of true, and then breaks the while loop
			userExists = true;
			break;
		}

	}

	// conditional checks the value of the bool userExists and adds the partner from the client if its true
	if(userExists) {
		// opens or creates the file that is represents the current user's partners
		fstream partnersFile(FILE_NAME_DIR + userName+ "_PartnerRequests.txt", ios::app | ios::out);

		// the String newPartner is inserted into the text file
        partnersFile << newPartner;

		// 1 is sent to the client to indicate that the server handled the String correctly
		writeConnection("1");
	}
	else {
		// 0 is sent to the client to indicate that the user sent as a partner doesn't exist
		writeConnection("0");
	}


}

void ConnectionHandler::option3Handler() {

	// opens/loads the partnersRequests file, and the partners file
	fstream usersFile(FILE_NAME_DIR + userName+ "_PartnerRequests.txt", ios::in);
	fstream partnersFile(FILE_NAME_DIR + userName+ "_Partners.txt", ios::app | ios::out);

	// creates Strings representing the currentLine and each user from the database file
	string currentLine, currentString ="";

	// while loop reads each line of the partnersRequests file
	while(getline(usersFile, currentLine) )	{

		// for loop checks each character for the '|' character which divides username from password in the file
		for(int i = 0; i < currentLine.length(); i++)
		{
			// once the '|' character is reached the for loop breaks
			if(currentLine.at(i) == '|')
				break;
			// the current character is added to the String for each a username
			currentString += currentLine.at(i);
		}

		// people are seperated by commas in the String based list
		currentString += ",";
	}

	// String is deliminated by a | character
	currentString += "|";

	// String is written to the client
	writeConnection(currentString);

	// String is read from the client, and bool is used to determine if the user accepted or rejected
	string newPartner = readConnection();
	bool accepted = newPartner.at(newPartner.size() - 1) == 'a';

	// conditional does nothing if 0 is returned, or the value of accepted is false
	if(newPartner=="0" || !accepted) {

    } else {

        newPartner = newPartner.substr(0, newPartner.length()-2);

        usersFile.clear();
        usersFile.seekg(0, ios::beg);
		// for loop checks each character for the '|' character which divides username from password in the file
		while(getline(usersFile, currentString) )	{
			// conditional compares the current username to the partner from the client client
			if(newPartner==currentString) {
				// String is written to the file
				partnersFile << currentString;
				break;
			}

		}	
	}
	
}

void ConnectionHandler::option4Handler() {

	fstream usersFile(FILE_NAME_DIR + userName+ "_Partners.txt", ios::in);
    string currentLine, currentString;

    // while loop loads each line of the file to currentLine one line at a time
    if(usersFile.is_open() && usersFile.good()) {
        while ( getline (usersFile,currentLine) ) {

            // for loop checks each character for the '|' character which divides username from password in the file
            for (int i = 0; i < currentLine.length(); i++) {
                // the current character is added to the String of usernames that will be written to the client
                currentString += currentLine.at(i);
            }
            // a comma added to sepereate usernames in the String
            currentString += ",";
        }
        // the String is terminated with a '|' character
        currentString += "|";

        cout << currentString << endl;
        // the String of usernames is written to the client via the writeConnection function
        writeConnection(currentString);
    }

}

void ConnectionHandler::option5Handler() {
		
	// sends a 1 to the client to indicate the server is ready recieve a String
	writeConnection("1");

	// stores the String from the client to newPartner
	string newPartner = readConnection();

	// opens/loads the users.txt file
	fstream usersFile(FILE_NAME_DIR + userName+ "_Partners.txt", ios::in);
	// creates Strings representing the currentLine and each user from the database file
	string currentLine, currentString;
	// bool represents if there's a match in the user.txt file
	bool userExists = false;

	// for loop checks each character for the '|' character which divides username from password in the file
	while(getline(usersFile, currentLine) )	{

		// creates an empty String for each line
		currentString = "";

		// for loop checks each character for the '|' character which divides username from password in the file
		for(int i = 0; i < currentLine.length(); i++)
		{
			// the current character is added to the String for each a username
			currentString += currentLine.at(i);
		}

		// conditional compares the current username to the partner from the client
		if(newPartner==currentString) {
			// bool assigned the value of true, and then breaks the while loop
			userExists = true;
			break;
		}

	}

	// conditional checks the value of the bool userExists and adds the partner from the client if its true
	if(userExists) {
		// 1 is sent to the client to indicate that the server handled the String correctly
		writeConnection("1");
		
		// stores the String from the client to newMessage
		string newMessage = readConnection();
		
		// opens or creates the file that is represents the current user's and partner's messages
		fstream messagesFile(FILE_NAME_DIR + userName+ "_to_" + newPartner + "_Messages.txt", ios::app | ios::out);

		// the String newMessage is inserted into the text file
        messagesFile << newMessage;
        
        writeConnection("1");
	}
	else {
		// 0 is sent to the client to indicate that the user sent as a partner doesn't exist
		writeConnection("0");
	}	
	
}

void ConnectionHandler::option6Handler() {
	// reads the partner sent from the client to a String 
	string selectedPartner = readConnection();

	fstream usersFile(FILE_NAME_DIR + selectedPartner + "_to_" + userName + "_Messages.txt", ios::in);
    string currentLine, currentString;

    // while loop loads each line of the file to currentLine one line at a time
    if(usersFile.is_open() && usersFile.good()) {
        int counter = 0;
        while ( getline (usersFile,currentLine) ) {
            counter++;
            currentString += counter + ". ";
            currentString += currentLine;
            // a comma added to sepereate usernames in the String
            currentString += ",";
        }
        // the String is terminated with a '|' character
        currentString += "|";

        cout << currentString << endl;
        // the String of usernames is written to the client via the writeConnection function
        writeConnection(currentString);
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
