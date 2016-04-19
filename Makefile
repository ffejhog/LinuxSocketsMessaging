CFLAGS=-std=c++11 #set compiler to use c++11 (to fix warning about const data)
CC=g++ #set compiler to be used


all: server client


client:
	$(CC) $(CFLAGS) -o Program6-Client.out Program-6-Client/Client.cpp Program-6-Client/Client.h  #Generates packaged binary file

server: main.o ConnectionHandler.o
	$(CC) $(CFLAGS)  -pthread -o Program6-Server.out Program-6-Server/main.o Program-6-Server/ConnectionHandler.o  #Generates packaged binary file
	if [ -d "data" ]; then echo "Dir exists"; else mkdir data; fi
	if [ -d "data/users.bin" ]; then echo "users.bin exists"; else echo "\n" >> data/users.bin; fi
main.o: Program-6-Server/main.cpp Program-6-Server/ConnectionHandler.h #Constucts main.o object
	$(CC) $(CFLAGS)  -pthread -c Program-6-Server/main.cpp -o Program-6-Server/main.o

ConnectionHandler.o: Program-6-Server/ConnectionHandler.cpp Program-6-Server/ConnectionHandler.h #Constucts fuelGauge.o object
	$(CC) $(CFLAGS)  -pthread -c Program-6-Server/ConnectionHandler.cpp -o Program-6-Server/ConnectionHandler.o

clean: #removes all compiled object files
	-rm Program-6-Server/*.o

cleanall: #Removes all but source files
	-rm *.out
	-rm Program-6-Server/*.o
	-rm -rf data/
