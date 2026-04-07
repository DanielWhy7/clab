CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O3
TARGET = program
SOURCES = main.c
OBJECTS = $(SOURCES:.c=.o)

.PHONY: clean run

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)
	mv $(TARGET) ..
	rm -f $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f ../$(TARGET)
	rm -f dean_call.txt reprimand.txt expulsion.txt

run: $(TARGET)
	../$(TARGET)
