CC = gcc
CFLAGS = -std=c11 -O2 -Wall -Wextra -I.
UTIL_DIR = utils
OBJ_DIR = obj
BIN_DIR = bin

# Collect all utils source and object files
UTIL_SRCS := $(wildcard $(UTIL_DIR)/*.c)
UTIL_OBJS := $(patsubst $(UTIL_DIR)/%.c, $(OBJ_DIR)/%.o, $(UTIL_SRCS))

# Chapter targets (add more as needed)
TARGETS := 02-insertion-sort

# Default target builds all
.PHONY: all clean utils $(TARGETS)
all: $(TARGETS)

# Target to build just the utils object files
utils: $(UTIL_OBJS)

# Rule to build chapter binaries
$(TARGETS): %: %/main.c $(UTIL_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $< $(UTIL_OBJS) -o $(BIN_DIR)/$@

# Rule to build each .o from utils/
$(OBJ_DIR)/%.o: $(UTIL_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
