CC=gcc
CFLAGS=-Wall -g -pthread -lrt
EXECUTABLE=sleeping_ta

all: $(EXECUTABLE)

$(EXECUTABLE): main.c
	$(CC) $(CFLAGS) -o $@ $<

clean: 
	rm $(EXECUTABLE)
