
TARGET := mkiptables

CC := $(shell ./compiler.sh)
CCLD := $(CC)

ifneq ($(OPTFLAGS),)
ifeq ($(DEBUG),YES)
ifeq ($(CC),clang)
OPTFLAGS := -Os
else
OPTFLAGS := -O3
endif
else
OPTFLAGS := -O0 -g
endif
endif

C_FILES := $(wildcard *.c)
OBJS := $(C_FILES:.c=.c.o)
HEADERS := $(wildcard *.h) version.h

EFFECTIVE_CFLAGS := $(CFLAGS) $(OPTFLAGS) -std=c99 -Wall

all: $(TARGET)

.PHONY: all clean

$(TARGET): $(OBJS)
	$(CCLD) $(LDFLAGS) $(OBJS) -o $@

%.c.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

version.h: version.sh
	./version.sh > version.h

clean:
	-rm -f $(OBJS) version.h $(TARGET)