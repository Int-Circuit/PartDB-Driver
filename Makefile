CC = gcc
CFLAGS = -Og -g -I. -Wall -Wextra -std=gnu23
LIBS = $(shell pkg-config --libs libfyaml libcurl libcjson)

SRCS := $(shell find . -name "*.c")
OBJS := $(patsubst %.c,.build/%.o,$(SRCS))
TARGET = .build/out

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p .build
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

.build/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf .build

.PHONY: all clean