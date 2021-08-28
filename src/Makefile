BIN = ../bin
BUILD = ../build
CC = gcc
FLAGS = -c -Wall

all: start server client
	@ echo "Building is complete"

start:
	@ echo "Start building"

server: $(BUILD)/server.o $(BUILD)/network.o 
	$(CC) -g $(BUILD)/server.o $(BUILD)/network.o -o $(BIN)/server

$(BUILD)/server.o: server.c network.h
	$(CC) $(FLAGS) server.c -o $(BUILD)/server.o

client: $(BUILD)/client.o $(BUILD)/network.o  network.h
	$(CC) -g $(BUILD)/client.o -o $(BIN)/client

$(BUILD)/client.o: client.c network.h
	$(CC) $(FLAGS) client.c -o $(BUILD)/client.o

$(BUILD)/network.o: network.c network.h
	$(CC) $(FLAGS) network.c -o $(BUILD)/network.o

clean:
	rm $(BUILD)/*
	rm $(BIN)/*