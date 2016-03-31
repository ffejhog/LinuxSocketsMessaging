CFLAGS=-std=c++11 -pthread #set compiler to use c++11 (to fix warning about const data)
CC=g++ #set compiler to be used


all: Program6-Server

Program6-Server: main.o ConnectionHandler.o
	$(CC) $(CFLAGS) -o Program6-Server.out main.o ConnectionHandler.o  #Generates packaged binary file
main.o: main.cpp ConnectionHandler.h #Constucts main.o object
	$(CC) $(CFLAGS) -c main.cpp

ConnectionHandler.o: ConnectionHandler.cpp ConnectionHandler.h #Constucts fuelGauge.o object
	$(CC) $(CFLAGS) -c ConnectionHandler.cpp

clean: #removes all compiled object files
	-rm *.o

cleanall: #Removes all but source files
	-rm *.out
	-rm *.o
