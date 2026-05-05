CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
LDFLAGS = -lpthread

TARGET = critical_sections
SRC = critical_sections.c

.PHONY: all run test clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

run: $(TARGET)
	./$(TARGET)

test: run

clean:
	rm -f $(TARGET)