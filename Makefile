#Composite makefile

#Variables
CC=g++
CC_FLAGS=-Wall -Werror -ansi -pedantic --std=c++0x
EXEC=/bin/rshell.out
SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:.cpp=.o)

$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC)

%.o: %.cpp
	$(CC) -c $(CC_FLAGS) $< -o $@



clean:
	rm -f $(EXEC) $(OBJECTS)
