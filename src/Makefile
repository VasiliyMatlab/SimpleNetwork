BIN = ../bin
BUILD = ../build
CC = gcc
FLAGS = -c -Wall

all: start server client
	@ echo "Building is complete"

start:
	@ echo "Start building"

server: $(BUILD)/server.o
	$(CC) -g $(BUILD)/server.o -o $(BIN)/server

$(BUILD)/server.o: server.c
	$(CC) $(FLAGS) server.c -o $(BUILD)/server.o

client: $(BUILD)/client.o
	$(CC) -g $(BUILD)/client.o -o $(BIN)/client

$(BUILD)/client.o: client.c
	$(CC) $(FLAGS) client.c -o $(BUILD)/client.o

clean:
	rm $(BUILD)/*
	rm $(BIN)/*