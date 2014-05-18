#Make has been modified to fit BOSS, rather than BOH shell
CC=gcc
CFLAGS=-c
LDFLAGS=
SOURCES=association.c history.c main.c prompt.c builtin.c launcher.c parser.c usrdat.c
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=../bin/BOSS

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@


