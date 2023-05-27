#Use GNU compiler
cc = gcc -g
CC = g++ -g

all: lab4

command.o: main.cc
	$(CC) -c main.cc
	
lab4:   command.o
	$(CC) -o lab4 main.o -ll

clean:
	rm -f lab4 *.o
