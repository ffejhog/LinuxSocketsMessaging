#include "Client.h"
/*
*	File Name: Client.cpp
*	Author: Kevin Tran, Jeff Neer, and Jeremy Polansky
*	Classes/Proceedures:
*		int main(int argc, char *argv[]) - constructor
*		mainClientHandler() - handles main login menu
*		userLogin() - Handles logging in a user
*		newUser() - Handles adding a new user
*		mainHandler() - Handles main menu for logged in user
*		option1Handler() - Option 1 handler
*		option2Handler() - Option 2 handler
*		option3Handler() - Option 3 handler
*		option4Handler() - Option 4 handler
*		option5Handler() - Option 5 handler
*		option6Handler() - Option 6 handler
*		readConnection() -  Reads data from socket
*		writeConnection(std::string dataToWrite) - Writes data to socket
*		passHash(const char* input, unsigned int seed) - Hashes a given character array
*/




/*	Procedure: main()
*	Author: Jeff
*	Description: Opens the socket for the client and binds to to the server.
*	Arguments:
*		argc - Input, Number of arguments
*		*argv[] - input, the array of command line arguments
*/

int main(int argc, char *argv[])
{
    int portNumber, numberOfCharacters;
    struct sockaddr_in server_address;
    struct hostent *server;


    if (argc < 3) {
        string ipAddress, portNumStr;
        cout << "What is the ip address or host name you wish to connect to?"<<endl;
        getline(cin,ipAddress);
        cout << "What is the port the server is listening on?"<<endl;
        getline(cin,portNumStr);
        portNumber = stoi(portNumStr);
        server = gethostbyname(ipAddress.c_str());
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
/*	Proceedure: mainClientHandler()
*	Author: Jeff
*	Description: Handles the main login menu for the user, allows the user to login or create a new account
*	Arguments:
*		none
*/

void mainClientHandler(){
    string userinput;

    do {
        cout << "Please Choose...  " << endl;
        cout << "1. Login" << endl;
        cout << "2. Create New User" << endl;
        cout << "3. Exit" << endl;

        getline(cin, userinput); //Get what command the user wants
        if(userinput==""){ //Check if server premeturly disconnected
            return;
        }
        int userinputnum = stoi(userinput);

        //Switch statement manges which function to class based on user input
        switch (userinputnum) {
            case 1:
                userLogin();
                break;
            case 2:
                newUser();
                break;
            case 3:
                writeConnection("3"); //Used to let the server know client is quiting
                return;
            default:
                cout << "That is not a valid option" << endl << endl;
        }
        }while(!authenticated); //Loops while user is not authenenticated

	mainHandler(); //Calls mainhandler when client is logged in.



}

/*	Proceedure: userLogin()
*	Author: Jeff
*	Description: Handles the process of authenticating a user
*	Arguments:
*		none
*/

void userLogin(){
    string userName;
    string password;

    writeConnection("1"); //Tell Server to prepare for login
    string serverResponse = readConnection(); //Retreive the servers response
    if(serverResponse == "1"){
        //server is good to receive
        cout << "Username: ";
        getline(cin, userName); //Ask for username
        cout << "Password: ";
        getline(cin, password);//Ask for password

        writeConnection(userName + "|" + to_string(passHash(password.c_str(),HASH_SEED))); //send the server the username|hased password combo
        serverResponse = readConnection(); //read servers response
        if(serverResponse == "1"){ //If "1" then login successful
            cout << "Success! Logging in User..." << endl;
            authenticated = true; //Set client to authenticated
            return;
        }else{ //Otherwise user does not exist
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

/*	Proceedure: newUser()
*	Author: Jeff
*	Description: Handles the process of creating a new user
*	Arguments:
*		none
*/

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
        //Retrieve username and password
		
        writeConnection(userName + "|" + to_string(passHash(password.c_str(),HASH_SEED))); //Write the new username and the new (hashed) password
        serverResponse = readConnection(); //Read server response
        if(serverResponse == "1"){ //User is accepted(not already already used)
            cout << "Success! Please login with your new account..." << endl;

            return;
        }else{//User declined(Already used)
            //User not valid
            cout << "ERROR: User already exists" << endl;
            return;
        }
    }else{
        //Server bad to receive, error
        cout << "Error from server, failed to add user - ERROR CODE: 1" << endl;
        return;
    }
}

/*	Proceedure: mainHandler()
*	Author: Jeff
*	Description: Handles the main menu for an authenticated user.
*	Arguments:
*		none
*/

void  mainHandler(){
    while(1) { //Run Until user quits
		//Main menu display
        cout << "Please Choose...  " << endl;
        cout << "1. List all users" << endl;
        cout << "2. Request a partnership with another user" << endl;
        cout << "3. View partner requests" << endl;
        cout << "4. View current partners" << endl;
        cout << "5. Send a message to a user" << endl;
        cout << "6. Retrieve a message from a user" << endl;
        cout << "7. Logout" << endl;
        string userinput;
        getline(cin, userinput); //Get user choice
        if(userinput==""){ //To prevent errors if server unexpectly diconnects
            return;
        }
        int userinputnum = stoi(userinput);
		//Witch statement that calls the various functions for each of the client commands
        switch (userinputnum) {
            case 1:
                writeConnection("1");
                option1Handler();
                break;
            case 2:
                writeConnection("2");
                option2Handler();
                break;
            case 3:
                writeConnection("3");
                option3Handler();
                break;
            case 4:
                writeConnection("4");
                option4Handler();
                break;
            case 5:
                writeConnection("5");
                option5Handler();
                break;
            case 6:
                writeConnection("6");
                option6Handler();
                break;
            case 7:
                writeConnection("7");
                return; //This command quits the program. 
            default:
                cout << "That is not a valid option" << endl << endl;
        }
    }
}




/*	Proceedure: void option1Handler()
 *	Author: Jeremy Polansky
 *	Description: Reads string info from server till a | deliminator, then formats and outputs to screen
 *	Arguments: NONE
 */
void option1Handler(){
    //assume server is now waiting to send list of registered users
    cout << endl << "Here is a list of all users: " << endl << endl;
    string str = readConnection();
    string newStr = "";
    for(int i=0; i<str.length();i++)
    {
        if(str.at(i)==',' || str.at(i)=='|')
        {
            newStr+='\n';

        }//end if
        else
        {
            newStr+=str.at(i);
        }//end else

    }//end for

    cout << newStr;



}//end option1Handler

/* Proceedure: Client.cpp
 * Author: Kevin Tran
 * Description: Client when ready will ask for name from server then send that name.
 *	Arguments: NONE
 */

void option2Handler(){
//PUT OPTION 2 STUFF HERE
    string status = readConnection();   //Read server connection
    string name;
    if (status == "1")  //If server send back 1, ready to receive name
    {
        cout << "Ready to receive name: " << endl;
        getline(cin, name);
        writeConnection(name);  //Send name back to server
    }
    else{
        cout << "ERROR!" <<endl;
	}

	status = readConnection();  //Server send back 1, shows confirmation
	if (status == "1")
    {
        cout << "You have send a partner request to " << name <<endl;
    }
    else{
        cout << "user does not Exist!" <<endl;
	}
}

/* Proceedure: Client.cpp
 * Author: Kevin Tran & Jeffrey Neer
 * Description: Client when ready will ask for name from server then send that name.
 *	Arguments: NONE
 */

void option3Handler() {
    //PUT OPTION 3 STUFF HERE
    string recievedlist = readConnection();
    string correctedList;
    string choice = "";

    for(int i=0; i<recievedlist.length();i++)
    {
        if(recievedlist.at(i)==',')
        {
            correctedList+='\n';

        }//end if
        else if(recievedlist.at(i)=='|'){

        } else {
            correctedList+=recievedlist.at(i);
        }//end else

    }//end for
    cout << correctedList;
    cout << "Type the name of a request to accept or deny(If you wish to do nothing, enter 0)" << endl;
    getline(cin, choice);
    if (choice == "0")
    {
        writeConnection("0");
    }
    else {
        string acceptRejectChoice;
        cout << "Do you wish to accept or deny " << choice <<"'s request? (a-Accept|r-Reject)" <<endl;
        getline(cin, acceptRejectChoice);
        writeConnection(choice+"|"+acceptRejectChoice);
        if(acceptRejectChoice=="a"){
            cout << choice <<" has been accepted"<<endl;
        }else{
            cout << choice <<" has been declined"<<endl;
        }
    }
}

/*	Proceedure: void option4Handler()
 *	Author: Jeremy Polansky
 *	Description: Reads string info from server till a | deliminator, then formats and outputs to screen
 *	Arguments: NONE
 */
void option4Handler(){
    //assume server is now waiting to send list of user's partners
    cout << endl << "Here is a list of all your partners: " << endl << endl;
    string str = readConnection();
    string newStr = "";
    for(int i=0; i<str.length();i++)
    {
        if(str.at(i)==',' || str.at(i)=='|')
        {
            newStr+='\n';
            
        }//end if
        else
        {
            newStr+=str.at(i);
        }//end else
        
    }//end for
    
    cout << newStr;
    
}//end option4Handler()

/*	Proceedure: void option5Handler()
 *	Author: Jeremy Polansky
 *	Description: sends a message to a partner and communicates with server to make sure it's sent
 *	Arguments: NONE?
 */
void option5Handler(){
    string option=readConnection();//assume this will be an integer value of one

    if(option!="1")
    {
        cout<<"Communication Error: Please restart everything and try again."<<endl;
    }//if not correct communication
    else if(option=="1") {
        cout << "Who are you sending a message to: " << endl;
        string userToSendTo;
        getline(cin, userToSendTo);
        writeConnection(userToSendTo);
        string response = readConnection();
        if (response == "1") {
            string mes;
            cout << "What is the message you would like to send" <<endl;
            getline(cin, mes);
            response = "0";
            int point = 0;
            int count;
            string newMes = "";
            /*while (point <= mes.length())//loop in case mes is longer than 255 characters
            {
                count = 0;
                while ((point <= mes.length()) &&
                       (count < 255))//sent strings will have free space in case | needs adding
                {

                    newMes += mes.at(point);//copy over char

                    if (point = mes.length())//if just added last char, put deliminator at end
                        newMes += "|";

                    point++;//update values
                    count++;

                }//another while loop
                */
                newMes = mes +"|";
                writeConnection(newMes);
                newMes = "";//clear in order to send more using same newMes string pointer
                //assume that this will not loop again if a | was added in nested while loop
            }//first while loop
            response = readConnection();//just reusing old string pointer
            if (response == "1") {
                cout << "Message Sent to Server." << endl;
            }
            else
                cout << "Server was not able not able to revice the message, but somehow was able to let us know" << endl;
        }//if correct communication
    else{
        cout << "User Not found, please try again" << endl;
    }
}//end option5Handler()

/*	Proceedure: void option6Handler()
 *	Author: Jeremy Polansky
 *	Description: Displays a message sent from a partner
 *	Arguments: NONE
 */
void option6Handler(){
    //assume server is now waiting to send list of user's partners
    cout << endl << "Here is all of your messages"<<endl;

    string str = readConnection();
    string newStr = "";
    for(int i=0; i<str.length();i++)
    {
        if(str.at(i)==',' || str.at(i)=='|')
        {
            newStr+='\n';
            
        }//end if
        else
        {
            newStr+=str.at(i);
        }//end else
    }//end for
    cout << newStr;
}//end option6Handler()

/*	Proceedure: readConnection()
*	Author: Jeff
*	Description: Allows a string to be read form the server by using a character buffer, then converting the result to a string
*	Arguments:
*		none
*/



std::string readConnection(){
    ssize_t numOfCharRead;
    bzero(buffer,512); //Zero out the buffer
    numOfCharRead = read(socketFileDescriptor,buffer,511); //read data from the server using the file descriptor connection into the character buffer.
    if (numOfCharRead < 0){//If no characters read something when wrong.
        cout << "ERROR reading from socket" << endl;
        return "";
    }
    std::string returnString(buffer);//Convert the character array to a string
    return returnString;//Return this string
}


/*	Proceedure: writeConnection(string)
*	Author: Jeff
*	Description: Allows a string to be written to the server by using a character buffer, then converting the result to a string
*	Arguments:
*		dataToWrite:
*			Direction - input
*			Type - string
*			Description - THe string representing the characters to write to the server.
*/


int writeConnection(std::string dataToWrite){
    ssize_t numOfCharRead;
    numOfCharRead = write(socketFileDescriptor,dataToWrite.c_str(),dataToWrite.length());//writes the string to the file discriptor representing the server.
    if (numOfCharRead < 0){//if no characters written there was a problem.
        cout << "ERROR writing to socket" << endl;
        return -1;
    }
    return 0;
}

/*	Proceedure: passHash()
*	Author: Jeff
*	Description: The method that hashes the password that is used to authenticate with the server.
*	Arguments:
*		input:
 *		    Direction - input
 *		    Type - Character array
 *		    Description - A character array to hash
*		seed:
 *		    Direction - input
 *		    Type - unsigned integer
 *		    Description - The seed used to has the value, must be the same for each version of the client database on the server
 *		returnValue:
 *		    Direction - output
 *		    Type - unsigned integer
 *		    Description - The generated hash value for the given character array
*/

unsigned int passHash(const char* input, unsigned int seed) {
    unsigned int hash = seed; //Set the hash value equal to the seed
    while (*input) //Used to iterate though the characters in the character array.
    {
        hash = hash * 101  +  *input++; //multiply the current has by a prime(101 in this case) then add the asscii value for the character in the nth place.
                                        //This value is placed back it hash. Then the loop repeats until every character is hashed.
    }
    return hash;
}
