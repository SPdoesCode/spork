CXX ?= clang++
CXXFLAGS ?= -std=c++17 -Wall -Wextra
SRC := $(wildcard src/*.cpp)
OBJ := $(SRC:.cpp=.o)
TARGET := spork

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET) $(OBJ)

.PHONY: all clean
