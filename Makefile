# 1. Platform Detection
# Query the host kernel name and strip trailing whitespace
UNAME_S := $(shell uname -s)

# 2. Conditional Compiler Assignment
ifeq ($(UNAME_S), Darwin)
    CC := clang
else ifeq ($(UNAME_S), Linux)
    CC := gcc
else
    CC := cc# Generic fallback fallback for other Unix-like systems
endif

# Project Information
VERSION := 0.8.0

# Compiler configuration
CFLAGS  := -std=c99 -Wall -Wextra -Wpedantic -Wshadow -O2
CFLAGS  += -DCALENDAR_VERSION='"$(VERSION)"'

TARGET  := calendar
SRC     := calendar.c

.PHONY: all clean run version

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

version: $(TARGET)
	./$(TARGET) --version

clean:
	rm -f $(TARGET)
