#цель: зависимость
#[tab] команда


BIN = ClientChatSM
SOURCES = main.c
HEADERS = main.h
CC := gcc

all:
	$(CC) $(SOURCES)  -lncurses -lrt  -lpthread -o $(BIN).out
$(BIN): $(HEADERS)

run: all
	./$(BIN).out

run2:
	./$(BIN).out
#clean:
#	rm -rf $(BIN) *.o $(BIN).out



















