# Makefile for tcpip_tool

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pthread
LDFLAGS = -lboost_system
SRC = main.cpp ArgsHelper.cpp TCPClient.cpp File_Input.cpp Keyboard_Input.cpp Menu.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = tcpip_tool

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJ) $(TARGET)
