
CC = gcc
FLAGS = -Wall -std=c11 

SRC_SER = $(wildcard server/*.c)
SRC_CLIENT = $(wildcard client/*.c)

BUILD_DIR = build

.PHONY : ALL SERVER CLIENT clean $(BUILD_DIR)

ALL : $(BUILD_DIR) SERVER CLIENT 

SERVER : $(SRC_SER)
	$(CC) $(FLAGS) $^ -o $(BUILD_DIR)/server

CLIENT : $(SRC_CLIENT)
	$(CC) $(FLAGS) $^ -o $(BUILD_DIR)/client

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean : 
	rm -rf $(BUILD_DIR)

