
CC=g++
CC_FLAGS=-Wall -Werror -ansi -pedantic

--std=c++0x

all: 
	mkdir -p ./bin
	$(CC) $(CC_FLAGS) ./src/main.cpp ./src/Execute.cpp -o ./bin/rshell
rshell:
	$(CC) $(CC_FLAGS) ./src/main.cpp ./src/Execute.cpp -o ./bin/rshell
clean:
	rm -rf ./bin
