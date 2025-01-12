CC = gcc
CFLAGS = -Wall -Wextra -Werror

TARGET = s21_grep

SRCS = s21_grep.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)

rebuild: clean all
