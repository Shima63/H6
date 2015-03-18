# Compiler

CC = g++

# Flags to the compiler

CFLAGS = -Wall

all: H5

H5: shima.cpp 
	$(CC) $(CFLAGS) shima.cpp -o shima

clean:
	rm -rf H5
