# Makefile


CC      := gcc
CFLAGS  := -Wall -Wextra -g -Iinclude -pthread

SRC_DIR := src
OBJ_DIR := build
BIN_DIR := bin
INC_DIR := include
DATA_DIR:= data

SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))


CLIENT_BIN := $(BIN_DIR)/client
SERVER_BIN := $(BIN_DIR)/server

#  Libraries to link
LDLIBS   := -lsqlite3 -lssl -lcrypto

.PHONY: all clean dirs

# Build  client and server
all: dirs $(CLIENT_BIN) $(SERVER_BIN)

# Create build/ and bin/ 
dirs:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BIN_DIR)
	mkdir -p $(DATA_DIR)

# Link client: client.o + database.o -> bin/client
$(CLIENT_BIN): $(OBJ_DIR)/client.o $(OBJ_DIR)/database.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

# Link server: server.o + database.o -> bin/server
$(SERVER_BIN): $(OBJ_DIR)/server.o $(OBJ_DIR)/database.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

# Compile any src/%.c into build/%.o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build/ and bin/
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
