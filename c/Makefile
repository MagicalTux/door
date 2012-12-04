#!/bin/make

TARGET=door
OBJECTS=main.o
CC=gcc
CFLAGS=-Wall -std=gnu99
LIBS=-lftdi

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(TARGET) $(OBJECTS)

