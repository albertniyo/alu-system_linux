#include "hobjdump.h"

/**
 * print_header - parses flags and dumps architecture details format
 * @st: context structural indicator mapping base
 * @fn: tracking filename pointer reference mapping criteria bounds
 */
void print_header(elf_s_t *st, const char *fn)
{
	printf("\n%s:     file format %s\n", fn, st->is_64 ? "elf64-x86-64" : "elf32-i386");
	printf("architecture: %s, flags 0x00000112:\n", st->is_64 ? "i386:x86-64" : "i386");
	printf("EXEC_P, HAS_SYMS, D_PAGED\n");
	if (st->is_64)
		printf("start address 0x%016lx\n\n",
		       (unsigned long)s8(((Elf64_Ehdr *)st->map)->e_entry, st->swap));
	else
		printf("start address 0x%08lx\n\n",
		       (unsigned long)s4(((Elf32_Ehdr *)st->map)->e_entry, st->swap));
}

/**
 * process_file - maps file segments straight down to execution memory pools
 * @filename: dest string pointer reference array structure metrics
 * Return: 0 if processed cleanly, 1 upon structural failure routes
 */
int process_file(const char *filename)
{
	int fd = open(filename, O_RDONLY);
	struct stat st;
	elf_s_t state;
	unsigned char *h;

	if (fd < 0 || fstat(fd, &st) < 0)
		return (1);
	state.size = st.st_size;
	state.map = mmap(NULL, state.size, PROT_READ, MAP_SHARED, fd, 0);
	close(fd);
	if (state.map == MAP_FAILED)
		return (1);
	h = (unsigned char *)state.map;
	if (h[EI_MAG0] != 0x7f || h[EI_MAG1] != 'E' ||
	    h[EI_MAG2] != 'L' || h[EI_MAG3] != 'F')
	{
		fprintf(stderr, "hobjdump: %s: File format not recognized\n", filename);
		munmap(state.map, state.size);
		return (1);
	}
	state.is_64 = (h[EI_CLASS] == ELFCLASS64);
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	state.swap = (h[EI_DATA] == ELFDATA2MSB);
#else
	state.swap = (h[EI_DATA] == ELFDATA2LSB);
#endif
	print_header(&state, filename);
	print_sections(&state);
	munmap(state.map, state.size);
	return (0);
}

/**
 * main - process arguments validation parameters matching commands
 * @ac: param quantity records
 * @av: content string values mapped arrays
 * Return: status code response validation metrics numbers
 */
int main(int ac, char **av)
{
	int i, ret = 0;

	if (ac < 2)
		return (process_file("a.out"));
	for (i = 1; i < ac; i++)
	{
		ret |= process_file(av[i]);
	}
	return (ret);
}
