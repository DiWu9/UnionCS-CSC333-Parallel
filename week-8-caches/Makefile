
#got some help from http://www.puxan.com/web/howto-write-generic-makefiles/

#MAC OS USERS UNCOMMENT THESE LINES
#LINUX USERS COMMENT THESE LINES
UNAME := $(shell uname -s)
ifeq ($(UNAME), Darwin)
CC=clang 
CFLAGS= -Xpreprocessor -fopenmp
LDFLAGS= -lm -lomp
endif
ifeq ($(UNAME), Linux)
CC=gcc
CFLAGS=-I. -fopenmp
LDFLAGS= -lm 
endif

#EXEC = hello 
OBJECTS = $(SOURCES:.c=.o)
SOURCES = $(wildcard *.c)
BINS=$(SOURCES:.c=)  #to compile a target, compile its c


# To obtain object files
%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@


all: $(BINS)

# Main target
$(EXEC): $(OBJECTS)
	$(CC)  $(OBJECTS) -o $(EXEC) $(LDFLAGS) 

clean:
	rm -f $(OBJECTS) $(BINS)
