CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -std=gnu99

all: hnm

hnm: main.c symbols.c utils.c
	$(CC) $(CFLAGS) main.c symbols.c utils.c -o hnm

clean:
	rm -f *.o

fclean: clean
	rm -f ./hnm

re: fclean all
