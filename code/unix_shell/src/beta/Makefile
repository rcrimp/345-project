# Paths for libraries and includes
COPTS =  -W -Wall -O2 -g

#source files
CFILES = parser.c process.c main.c 

#compiler
CC = gcc

TARGET = shell

default: clean go

go:
	$(CC) $(COPTS) $(CFILES) -o $(TARGET)

clean:
	rm -f $(TARGET) core
