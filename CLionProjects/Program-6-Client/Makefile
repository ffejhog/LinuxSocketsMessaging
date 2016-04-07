CFLAGS=-std=c++11 #set compiler to use c++11 (to fix warning about const data)
CC=g++ #set compiler to be used


all: Program6-Client

Program6-Client:
	$(CC) $(CFLAGS) -o Program6-Client.out Client.cpp Client.h  #Generates packaged binary file

cleanall: #Removes all but source files
	-rm *.out
