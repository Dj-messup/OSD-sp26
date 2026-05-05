CXX = g++
CXXFLAGS = -std=c++20 -O3 -Wall -Wextra -march=native -DNDEBUG

TARGET = memory_optimization
SRC = memory_optimization.cpp

.PHONY: all run small test clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

run: $(TARGET)
	./$(TARGET)

small: $(TARGET)
	./$(TARGET) 500000 30

test: run

clean:
	rm -f $(TARGET)
