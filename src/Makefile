BIN = ../bin
BUILD = ../build
CC = gcc
FLAGS = -c -Wall

all: start server
	@ echo "Building is complete"

start:
	@ echo "Start building"

server: $(BUILD)/server.o
	$(CC) -g $(BUILD)/server.o -o $(BIN)/server

$(BUILD)/server.o: server.c
	$(CC) $(FLAGS) server.c -o $(BUILD)/server.o

clean:
	rm $(BUILD)/*
	rm $(BIN)/*