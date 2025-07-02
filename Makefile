# Detect platform
ifeq ($(OS),Windows_NT)
    PLATFORM := Windows
    CC := gcc
    EXE_EXT := .exe
    RM := rmdir /s /q
    PATHSEP := \\
else
    PLATFORM := Linux
    CC := gcc
    EXE_EXT :=
    RM := rm -rf
    PATHSEP := /
endif

CFLAGS = -std=c11 -O2 -Wall -Wextra -I.
UTIL_DIR = utils
OBJ_DIR = obj
BIN_DIR = bin

# Collect all utils source and object files
UTIL_SRCS := $(wildcard $(UTIL_DIR)/*.c)
UTIL_OBJS := $(patsubst $(UTIL_DIR)/%.c, $(OBJ_DIR)/%.o, $(UTIL_SRCS))

# Chapter targets (add more as needed)
TARGETS := 02-insertion-sort 02-merge-sort 02-binary-search 04-max-subarray

# Default target builds all
.PHONY: all clean utils $(TARGETS)
all: $(TARGETS)

# Target to build just the utils object files
utils: $(UTIL_OBJS)

# Rule to build chapter binaries
$(TARGETS): %: %/main.c $(UTIL_OBJS)
ifeq ($(PLATFORM),Windows)
	@if not exist $(subst /,\,$(BIN_DIR)) mkdir $(subst /,\,$(BIN_DIR))
else
	@mkdir -p $(BIN_DIR)
endif
	$(CC) $(CFLAGS) $< $(UTIL_OBJS) -o $(BIN_DIR)/$@$(EXE_EXT)

# Rule to build each .o from utils/
$(OBJ_DIR)/%.o: $(UTIL_DIR)/%.c
ifeq ($(PLATFORM),Windows)
	@if not exist $(subst /,\,$(OBJ_DIR)) mkdir $(subst /,\,$(OBJ_DIR))
else
	@mkdir -p $(OBJ_DIR)
endif
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up
clean:
ifeq ($(PLATFORM),Windows)
	@if exist $(OBJ_DIR) $(RM) $(subst /,\,$(OBJ_DIR))
	@if exist $(BIN_DIR) $(RM) $(subst /,\,$(BIN_DIR))
else
	$(RM) $(OBJ_DIR) $(BIN_DIR)
endif

# Show detected platform (optional - for debugging)
info:
	@echo Platform: $(PLATFORM)
	@echo Compiler: $(CC)
	@echo Executable extension: $(EXE_EXT)