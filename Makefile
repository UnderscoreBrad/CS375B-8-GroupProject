.DEFAULT_GOAL := create
CC = g++
FLAGS = -g -Ofast -Wall -Wextra -std=c++17 -pedantic
OBJECTS = hashing_test.o
EXECUTABLE = hashing_test
NAME = CS375B-8

all:    clean create

create:	compile
	$(CC) $(OBJECTS) -o $(EXECUTABLE) -lpthread

compile:	$(EXECUTABLE).cpp
	$(CC) $(FLAGS) -c $(EXECUTABLE).cpp

clean:
	rm -f -r *.o *.exe *.cpp~ $(EXECUTABLE) .vscode
	
tar: clean
	cd .. && tar --exclude-vcs -cvzf $(NAME).tar.gz $(NAME) && cd -