CC = gcc
CFLAGS = -Wall -Wextra -Werror

TARGET = s21_cat

SRCS = s21_cat.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)

rebuild: clean all
