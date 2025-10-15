CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET = dungeon_crawler
SRC = main.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)
