CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -Iinclude

SRCS = src/main.cpp src/Logger.cpp src/FileHandler.cpp src/FileManager.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = app

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
