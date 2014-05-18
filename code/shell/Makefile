# Paths for libraries and includes
COPTS =  -Wall -std=c++0x

#source files
CFILES = process.cpp main.cpp

#compiler
CC = g++

TARGET = shell

default: clean go

go:
	$(CC) $(COPTS) $(CFILES) -o $(TARGET)

clean:
	rm -f $(TARGET) core
