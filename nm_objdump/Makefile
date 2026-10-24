CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -std=gnu99

NM_SRC = hnm_src/main.c hnm_src/symbols.c hnm_src/utils.c
NM_OBJ = $(NM_SRC:.c=.o)

OBJDUMP_SRC = hobjdump_src/main.c hobjdump_src/sections.c hobjdump_src/utils.c
OBJDUMP_OBJ = $(OBJDUMP_SRC:.c=.o)

.PHONY: all clean fclean re

all: hnm hobjdump

hnm: $(NM_OBJ)
	$(CC) $(CFLAGS) $(NM_OBJ) -o hnm

hobjdump: $(OBJDUMP_OBJ)
	$(CC) $(CFLAGS) $(OBJDUMP_OBJ) -o hobjdump

clean:
	rm -f $(NM_OBJ) $(OBJDUMP_OBJ)

fclean: clean
	rm -f hnm hobjdump

re: fclean all
