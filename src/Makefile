BIN = ../bin
BUILD = ../build
CC = gcc
FLAGS = -c -Wall


all: start server client
	@ echo "Building is complete"

start:
	@ echo "Start building"


server: $(BUILD)/server.o $(BUILD)/network.o $(BUILD)/conditions.o $(BUILD)/internal.o network.h conditions.h internal.h
	$(CC) -g $(BUILD)/server.o $(BUILD)/network.o $(BUILD)/conditions.o $(BUILD)/internal.o -o $(BIN)/server

$(BUILD)/server.o: server.c network.h conditions.h internal.h
	$(CC) $(FLAGS) server.c -o $(BUILD)/server.o

client: $(BUILD)/client.o $(BUILD)/network.o $(BUILD)/conditions.o $(BUILD)/internal.o network.h conditions.h internal.h
	$(CC) -g $(BUILD)/client.o $(BUILD)/network.o $(BUILD)/conditions.o $(BUILD)/internal.o -o $(BIN)/client

$(BUILD)/client.o: client.c network.h conditions.h internal.h
	$(CC) $(FLAGS) client.c -o $(BUILD)/client.o


$(BUILD)/conditions.o: conditions.c conditions.h
	$(CC) $(FLAGS) conditions.c -o $(BUILD)/conditions.o

$(BUILD)/network.o: network.c network.h
	$(CC) $(FLAGS) network.c -o $(BUILD)/network.o

$(BUILD)/internal.o: internal.c internal.h
	$(CC) $(FLAGS) internal.c -o $(BUILD)/internal.o


clean:
	rm $(BUILD)/*
	rm $(BIN)/*