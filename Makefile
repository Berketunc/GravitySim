# Variables
CC = g++
CFLAGS = -Wall -std=c++11
LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# Name of your output executable
TARGET = gravity_sim

# Default target
all: $(TARGET)

$(TARGET): main.cpp
	$(CC) $(CFLAGS) main.cpp -o $(TARGET) $(LIBS)

# Clean up the executable
clean:
	rm -f $(TARGET)
