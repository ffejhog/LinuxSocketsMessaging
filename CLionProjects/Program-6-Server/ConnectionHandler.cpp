/*
*	File Name: Player.cpp
*	Author: Daniel Lecheler & Jeff Near
*	Classes/Proceedures:
*		ConnectionHandler(int) - constructor
*		loginHandler() -
*		newUser() -
*		checkIfAuthenticated() -
*		mainHandler() -
*		option1Handler() -
*		option2Handler() -
*		option3Handler() -
*		option4Handler() -
*		option5Handler() -
*		option6Handler() -
*/
#include "ConnectionHandler.h"

/*	Proceedure: conncetionHandler()
*	Author: Jeff
*	Description: This is the default constructor for each connected user. It works with the client to authenticate the user. Once the user is authenticated,
*	             this class is used to handle all of the communication between client and server
*	Arguments:
*		ConnectedClientFileDescriptor:
*			Direction: Input
*			Type: int
*			Description: This is the file Discriptor of the socket connection between the client and the server
*/
ConnectionHandler::ConnectionHandler(int ConnectedClientFileDescriptor){
    ClientFileDescriptor = ConnectedClientFileDescriptor; //Store the socket connection descriptor for this client into the class for easy access.


    do {
        std::string enteredCommand = readConnection(); //Wait for client to send command
        if(enteredCommand==""){ //To protect against client disconnection unexpectedly
            return;
        }
        int enteredCommandnum = stoi(enteredCommand);

        switch (enteredCommandnum) { //THis switch statement takes the entered command and goes to the approprate method to handle.
            case 1:
                loginHandler();
                break;
            case 2:
                newUser();
                break;
            default:
                return;
        }
    }while(!authenticated); //Loop while the user is not logged in.

}

/*	Proceedure: loginHandler()
*	Author: Jeff
*	Description: This class handles the login process by comparing the username and password combination sent from the client against the users.bin file
*	Arguments:
*		none
*/
void ConnectionHandler::loginHandler(){
    writeConnection("1"); // Letting client know Server is ready to recieve

    string enteredUserName = readConnection(); //Read username password combination

    fstream userDatabase;
    userDatabase.open(FILE_NAME_DIR+"users.bin", ios::in); //Open users.bin file
    string readLine;


    if (userDatabase.is_open()){
        while ( getline (userDatabase,readLine) ) //Read each file of file
        {
            if(readLine == enteredUserName) { //If user|pass combo found, authenticate
                authenticated = true;

                for(int i = 0; i < enteredUserName.length(); i++)
                {
                    // once the '|' character is reached the for loop breaks
                    if(enteredUserName.at(i) == '|')
                        break;
                    // the current character is added to the String for each a username
                    userName += enteredUserName.at(i); //Retreieve just the username for the class.
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
        writeConnection("0");//User is not authenticated, let client know
    }




}

/*	Proceedure: newUser()
*	Author: Jeff
*	Description: Allows the client to create a new user
*	Arguments:
*		none
*/
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

/*	Proceedure: checkIfAuthenticated()
*	Author: Jeff
*	Description: SImply returns whether or not the user is authenticated
*	Arguments:
*		none
*/
bool ConnectionHandler::checkIfAuthenticated(){
    return authenticated;
}

/*	Proceedure: mainHandler()
*	Author: Jeff
*	Description: Handles the main menu for an authenticated user.
*	Arguments:
*		none
*/
void ConnectionHandler::mainHandler() {
        std::string enteredCommand;
        while (1){ //Loop forever unless user says to quit
            enteredCommand = "0";
            enteredCommand = readConnection(); //Read what command the user wishes to do
            if(enteredCommand==""){ //Check if client disconnected unexpectedly
                return;
            }
            int enteredCommandnum = std::stoi(enteredCommand);

            switch(enteredCommandnum){ //Check for success
                case 0:
                    cout << "ERROR on client read" <<endl;
                    closeConnection();
	                break;
            //All of these below simply check which method to call based on the input
                case 1:
			        option1Handler();
                    break;
                case 2:
			        option2Handler();
                    break;
                case 3:
                    option3Handler();
                    break;
                case 4:
                    option4Handler();
                    break;
                case 5:
                    option5Handler();
                    break;
                case 6:
                    option6Handler();
                default:
                    return;
            }
    }




}

/*	Proceedure: option1Handler()
*	Author: Daniel
*	Description: the mehtod communicates between the client and server to display the list of users
*	Arguments:
*		none
*/
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

/*	Proceedure: option2Handler()
*	Author: Daniel
*	Description: the mehtod communicates between the client and server to send a partner request from the current user, to another user
*	Arguments:
*		none
*/
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
		fstream partnersFile(FILE_NAME_DIR + newPartner + "_PartnerRequests.txt", ios::app | ios::out);

		// the String newPartner is inserted into the text file
        partnersFile << userName;

		// 1 is sent to the client to indicate that the server handled the String correctly
		writeConnection("1");
	}
	else {
		// 0 is sent to the client to indicate that the user sent as a partner doesn't exist
		writeConnection("0");
	}


}

/*	Proceedure: option3Handler()
*	Author: Daniel
*	Description: the mehtod communicates between the client and server to display the list of a user's partner requests from other users, and then ask them to accept or reject a specific request
*	Arguments:
*		none
*/
void ConnectionHandler::option3Handler() {

	// opens/loads the partnersRequests file, and the partners file
	fstream usersFile(FILE_NAME_DIR + userName + "_PartnerRequests.txt", ios::in);
	fstream partnersFile(FILE_NAME_DIR + userName + "_Partners.txt", ios::app | ios::out);

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
	writeConnection(currentString+"\n");

	// String is read from the client, and bool is used to determine if the user accepted or rejected
	string newPartner = readConnection();
	bool accepted = newPartner.at(newPartner.size() - 1) == 'a';

	// conditional does nothing if 0 is returned, or the value of accepted is false
	if(newPartner=="0") {
    }
	else if(!accepted)
	{
		// newPartner has the |a or |r removed
        newPartner = newPartner.substr(0, newPartner.length()-2);
        // a temp String and temp file are created
		string tempString = "";
        fstream temp(FILE_NAME_DIR + userName + "temp.txt", ios::app | ios::out);

		// the for loop copies the values from the partner text file and adds them to the temp text file, excluding the current value of newPartner
		for(int i = 0; i < currentString.length(); i++) {
			if(tempString.compare(newPartner)) {
				i++;
				tempString = "";
			}
			else if(currentString.at(i) == ',' || currentString.at(i) == '|') {
				temp << tempString;
				tempString = "";
			}
			tempString += currentString.at(i);
		}

		// replaces the partner request file with the temp file, that's a copy of the original with the partner request that was accepted removed

		usersFile.close();
    	temp.close();
        string fileName1 = FILE_NAME_DIR + userName+ "_PartnerRequests.txt";
        string filename2 = FILE_NAME_DIR + userName + "temp.txt";
        remove(fileName1.c_str());
    	rename(filename2.c_str(),fileName1.c_str());

	}
	else {

		// newPartner has the |a or |r removed
        newPartner = newPartner.substr(0, newPartner.length()-2);
        // a temp String and temp file are created
		string tempString = "";
        fstream temp(FILE_NAME_DIR + userName + "temp.txt", ios::app | ios::out);
        fstream newPartnerPartners(FILE_NAME_DIR + newPartner + "_Partners.txt", ios::app | ios::out);

        // the for loop copies the values from the partner text file and adds them to the temp text file, excluding the current value of newPartner
		for(int i = 0; i < currentString.length(); i++) {
			if(tempString.compare(newPartner)) {
				i++;
				tempString = "";
			}
			else if(currentString.at(i) == ',' || currentString.at(i) == '|') {
				temp << tempString;
				tempString = "";
			}
			tempString += currentString.at(i);
		}

        usersFile.clear();
        usersFile.seekg(0, ios::beg);
		// for loop checks each character for the '|' character which divides username from password in the file
		while(getline(usersFile, currentString) )	{
			// conditional compares the current username to the partner from the client client
			if(newPartner==currentString) {
				// String is written to the file
				partnersFile << currentString;
                newPartnerPartners << currentString;
				break;
			}

		}

	// replaces the partner request file with the temp file, that's a copy of the original with the partner request that was accepted removed
	usersFile.clear();
	usersFile.close();
    temp.close();
    string fileName = FILE_NAME_DIR + userName+ "_PartnerRequests.txt";
    string fileName2 = FILE_NAME_DIR + userName+ "temp.txt";
    remove(fileName.c_str());
    rename(fileName2.c_str(),fileName.c_str());


	}

}

/*	Proceedure: option4Handler()
*	Author: Daniel
*	Description: the mehtod communicates between the client and server to display the list of partners for a specific user
*	Arguments:
*		none
*/
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
    usersFile.close();
}

/*	Proceedure: option5Handler()
*	Author: Daniel
*	Description: the mehtod communicates between the client and server to send a message to a partner
*	Arguments:
*		none
*/
void ConnectionHandler::option5Handler() {

	// sends a 1 to the client to indicate the server is ready recieve a String
	writeConnection("1");

	// stores the String from the client to newPartner
	string newPartner = readConnection();

	// opens/loads the users.txt file
	fstream usersFile(FILE_NAME_DIR + "users.bin", ios::in);
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
		if(newPartner==currentString) {
			// bool assigned the value of true, and then breaks the while loop
			userExists = true;
			break;
		}


	}
    usersFile.close();
	// conditional checks the value of the bool userExists and adds the partner from the client if its true
	if(userExists) {
		// 1 is sent to the client to indicate that the server handled the String correctly
		writeConnection("1");

		// stores the String from the client to newMessage
		string newMessage = readConnection();

		// opens or creates the file that is represents the current user's and partner's messages
		fstream messagesFile(FILE_NAME_DIR + newPartner + "_Messages.txt", ios::app | ios::out);


		// the String newMessage is inserted into the text file
        messagesFile << userName <<"(" << currentDateTime() << "): " << newMessage << endl;

        writeConnection("1");
        messagesFile.close();
	}
	else {
		// 0 is sent to the client to indicate that the user sent as a partner doesn't exist
		writeConnection("0");
	}



}

/*	Proceedure: option6Handler()
*	Author: Daniel
*	Description: the mehtod communicates between the client and server to display the messages between a partner and a specific partner
*	Arguments:
*		none
*/
void ConnectionHandler::option6Handler() {
	// reads the partner sent from the client to a String

	fstream usersFile(FILE_NAME_DIR + userName + "_Messages.txt", ios::in);
    string currentLine, currentString;

    // while loop loads each line of the file to currentLine one line at a time
    if(usersFile.is_open()) {
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

    }
    writeConnection(currentString);
    usersFile.clear();
    usersFile.close();
}


/*	Proceedure: closeConnection(const char)
*	Author: Jeff
*	Description:
*	Arguments: CLoses the file discriptor for the client, terminating the connection
*/
void ConnectionHandler::closeConnection() {
    close(ClientFileDescriptor);
}

/*	Proceedure: readConnection()
*	Author: Jeff
*	Description: Allows a string to be read form the client by using a character buffer, then converting the result to a string
*	Arguments:
*		none
*/
std::string ConnectionHandler::readConnection(){
    bzero(buffer,512); //zero out the buffer
    numOfCharRead = read(ClientFileDescriptor,buffer,511); //read data from the client using the file discriptor connection into the characer buffer.
    if (numOfCharRead < 0){ //If no characers read somthing when wrong.
        cout << "ERROR reading from socket" << endl;
        return "";
    }
    std::string returnString(buffer); //Convert the character array to a string
    return returnString; //Return this string
}

/*	Proceedure: writeConnection(string)
*	Author: Jeff
*	Description: Allows a string to be written to the client by using a character buffer, then converting the result to a string
*	Arguments:
*		dataToWrite:
*			Direction - input
*			Type - string
*			Description - THe string representing the characters to write to the client.
*/
int ConnectionHandler::writeConnection(std::string dataToWrite){
    numOfCharRead = write(ClientFileDescriptor,dataToWrite.c_str(),dataToWrite.length()); //writes the string to the file discriptor representing the client.
    if (numOfCharRead < 0){ //if no characters written there was a problem.
        cout << "ERROR writting to socket" << endl;
        return -1;
    }
    return 0;
}

/*	Proceedure: currentDateTime()
*	Author: Jeff
*	Description: Returns the current date and time
*	Arguments:
*		std::string:
*			Direction - output
*			Type - string
*			Description - The data time combination representing the current time
*/

const std::string ConnectionHandler::currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct; //Structure built for time storage(Located in time.h)
    char       buf[80]; //Create temporary character buffer for time values
    tstruct = *localtime(&now); //Set the time structure equal to the current time (in epoch time) to values in time struct

    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct); //Shifts the time value to Year - Month - Day - Time format

    return buf;
}
