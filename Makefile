CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -std=c17
INCLUDE = -Iinclude
LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -llua

SRC = $(wildcard src/*.c) $(wildcard src/**/*.c)
OBJ = $(SRC:.c=.o)
BUILD = build

all: dirs voxeng

dirs:
	mkdir -p $(BUILD) src include tests

run: voxeng
	$(BUILD)/voxeng

voxeng: $(OBJ)
	$(CC) $? $(CFLAGS) $(INCLUDE) $(LIBS) -o $(BUILD)/$@

%.o: %.c
	$(CC) $< $(CFLAGS) $(INCLUDE) $(LIBS) -o $@ -c

clean:
	rm -Rf $(BUILD)/voxeng $(OBJ)
