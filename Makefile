CC = gcc
CFLAGS = -Wall -Wextra

all: tensor

tensor: tensor.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f tensor