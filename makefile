# Default compilers
CC ?= gcc
CXX ?= g++

TRACE ?= 0

# Choose compiler based on TRACE
ifeq ($(TRACE),1)
    COMPILER = $(CXX)
    CFLAGS = -g -Wall -DMTR_ENABLED -O3 -DTRACE=1
else
    COMPILER = $(CC)
    CFLAGS = -g -Wall -DMTR_ENABLED -O3
endif

TARGET = main
SRC = main.c minitrace.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(COMPILER) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

rebuild: clean all