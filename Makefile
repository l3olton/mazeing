CC = gcc
CFLAGS = -Wall
LDLIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

SRC_DIR = src
BUILD_DIR = build

mazeing: $(SRC_DIR)/main.c
	 # $(SRC_DIR)/
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ $^ $(LDLIBS)

debug: $(SRC_DIR)/main.c
	$(CC) $(CFLAGS) -g -o $(BUILD_DIR)/$@ $^ $(LDLIBS)

clean:
	rm -f mazeing
