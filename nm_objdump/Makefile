CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -std=gnu99

SRC = hnm_src/main.c hnm_src/symbols.c hnm_src/utils.c
OBJ = $(SRC:.c=.o)
NAME = hnm

.PHONY: all clean fclean re

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
