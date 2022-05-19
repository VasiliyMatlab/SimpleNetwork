BIN = ../bin
BUILD = ../build
CC = gcc
СFLAGS = -c -Wall -Werror -std=gnu17 -O3
LDFLAGS = -g -pthread -Wall -Werror -std=gnu17 -O3


all: start server client
	@ echo "Building is complete"

start:
	@ echo "Start building"
	mkdir -pv ../build
	mkdir -pv ../bin


server: $(BUILD)/server.o $(BUILD)/network.o $(BUILD)/conditions.o $(BUILD)/internal.o $(BUILD)/parser.o network.h conditions.h internal.h parser.h
	$(CC) $(LDFLAGS) $(BUILD)/server.o $(BUILD)/network.o $(BUILD)/conditions.o $(BUILD)/internal.o $(BUILD)/parser.o -o $(BIN)/server

$(BUILD)/server.o: server.c network.h conditions.h internal.h
	$(CC) $(СFLAGS) server.c -o $(BUILD)/server.o

client: $(BUILD)/client.o $(BUILD)/network.o $(BUILD)/conditions.o $(BUILD)/internal.o $(BUILD)/parser.o network.h conditions.h internal.h parser.h
	$(CC) $(LDFLAGS) $(BUILD)/client.o $(BUILD)/network.o $(BUILD)/conditions.o $(BUILD)/internal.o $(BUILD)/parser.o -o $(BIN)/client

$(BUILD)/client.o: client.c network.h conditions.h internal.h
	$(CC) $(СFLAGS) client.c -o $(BUILD)/client.o


$(BUILD)/conditions.o: conditions.c conditions.h internal.h parser.h
	$(CC) $(СFLAGS) conditions.c -o $(BUILD)/conditions.o

$(BUILD)/network.o: network.c network.h
	$(CC) $(СFLAGS) network.c -o $(BUILD)/network.o

$(BUILD)/internal.o: internal.c internal.h
	$(CC) $(СFLAGS) internal.c -o $(BUILD)/internal.o

$(BUILD)/parser.o: parser.c parser.h
	$(CC) $(СFLAGS) -pthread parser.c -o $(BUILD)/parser.o


archiving:
	cd $(BIN)/ && tar -zcvf Binaries.tar.gz client server

clean:
	rm -rv $(BUILD)
	rm -rv $(BIN)