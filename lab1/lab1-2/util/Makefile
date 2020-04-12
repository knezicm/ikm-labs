TARGET = libutil.so
CC = arm-linux-gnueabihf-gcc
CFLAGS = -fPIC
LDFLAGS = -shared

INCLUDE = -I./include
LIBS = 
LIBDIR = 

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


