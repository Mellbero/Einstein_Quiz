CC = gcc
CFLAGS = -g -Wall -DMTR_ENABLED -O3 

TARGET = main
SRC = main.c minitrace.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

rebuild: clean all