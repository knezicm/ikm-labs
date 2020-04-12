TARGET = hello
CC = arm-linux-gnueabihf-gcc
CFLAGS = 
LDFLAGS = 

INCLUDE = -I../util/include
LIBS = -lutil
LIBDIR = -L../util/lib

SRC = $(wildcard src/*.c)

OBJ = $(patsubst %.c, %.o, $(SRC))

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBDIR) $(LIBS)

%.o: %.c
	$(CC) -c -o $@ $< $(CFLAGS) $(INCLUDE)

.PHONY: clean
clean:
	rm -f src/*.o $(TARGET)

