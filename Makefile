CC = g++
FLAGS = -g -Ofast -Wall -Wextra -std=c++17 -pedantic
SRC = $(wildcard *.cpp)
OBJ = $(SRC:.cpp=.o)
EXECUTABLE = hashing_test
NAME = CS375B-8

hashing_test: $(OBJ)
	$(CC) -o $@ $^ $(FLAGS) -lpthread

clean:
	rm -fr *.o *.exe *.cpp~ $(EXECUTABLE) .vscode
	
tar: clean
	cd .. && tar --exclude-vcs -cvzf $(NAME).tar.gz $(NAME) && cd -