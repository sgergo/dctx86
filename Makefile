EXECUTABLE := $(shell basename `pwd`)

all: *.c *.h
	gcc -Wall -O4 *.c -o $(EXECUTABLE) -lm

clean:
	rm -f $(EXECUTABLE)
