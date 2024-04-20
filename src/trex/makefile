# Define the compiler
CC = gcc

# Define any compiler flags you want to use
CFLAGS = -Wall -Wextra -g

# Define the output executable
TARGET = trex

# Define the source files
SOURCES = src/trex/util/tokenizer.c \
          src/trex/util/system/clear.c \
          src/trex/util/operations.c \
          src/trex/core/util/varmap.c \
          src/trex/core/symbol/symbol.c \
          src/trex/core/trex/trex-stack.c \
          src/trex/core/trex/trex.c \
          src/trex/core/trex-parser.c \
          src/trex/components.c \
          src/trex/main.c

# Define the object files
OBJECTS = $(SOURCES:.c=.o)

# Default target
all: $(TARGET)

# Rule to link the object files into the executable
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)

# Rule to compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target
clean:
	rm -f $(TARGET) $(OBJECTS)

.PHONY: all clean