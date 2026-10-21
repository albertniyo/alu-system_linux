#include "hnm.h"

/**
 * parse_64 - helper to parse 64-bit sections
 * @st: exec framework property memory target track definitions
 * Return: 1 if found, 0 otherwise
 */
int parse_64(elf_s_t *st)
{
	int i, found = 0;
	char *s_strs;
	Elf64_Ehdr *eh = st->map;
	Elf64_Shdr *sh = (Elf64_Shdr *)((char *)st->map +
					s8(eh->e_shoff, st->swap));

	for (i = 0; i < s2(eh->e_shnum, st->swap); i++)
	{
		if (s4(sh[i].sh_type, st->swap) != SHT_SYMTAB)
			continue;
		found = 1;
		s_strs = (char *)st->map +
			s8(sh[s4(sh[i].sh_link, st->swap)].sh_offset,
			   st->swap);
		loop_syms(st, (char *)st->map +
			  s8(sh[i].sh_offset, st->swap), s_strs,
			  s8(sh[i].sh_size, st->swap) /
			  s8(sh[i].sh_entsize, st->swap),
			  s8(sh[i].sh_entsize, st->swap), sh);
	}
	return (found);
}

/**
 * parse_32 - helper to parse 32-bit sections
 * @st: exec framework property memory target track definitions
 * Return: 1 if found, 0 otherwise
 */
int parse_32(elf_s_t *st)
{
	int i, found = 0;
	char *s_strs;
	Elf32_Ehdr *eh = st->map;
	Elf32_Shdr *sh = (Elf32_Shdr *)((char *)st->map +
					s4(eh->e_shoff, st->swap));

	for (i = 0; i < s2(eh->e_shnum, st->swap); i++)
	{
		if (s4(sh[i].sh_type, st->swap) != SHT_SYMTAB)
			continue;
		found = 1;
		s_strs = (char *)st->map +
			s4(sh[s4(sh[i].sh_link, st->swap)].sh_offset,
			   st->swap);
		loop_syms(st, (char *)st->map +
			  s4(sh[i].sh_offset, st->swap), s_strs,
			  s4(sh[i].sh_size, st->swap) /
			  s4(sh[i].sh_entsize, st->swap),
			  s4(sh[i].sh_entsize, st->swap), sh);
	}
	return (found);
}

/**
 * parse_sections - inspects section tables looking for active symbol entries
 * @st: exec framework property memory target track definitions
 * @fn: dest string pointer reference array structure metrics
 * Return: status evaluation check numbers
 */
int parse_sections(elf_s_t *st, const char *fn)
{
	int found = 0;

	if (st->is_64)
		found = parse_64(st);
	else
		found = parse_32(st);

	if (!found)
		fprintf(stderr, "./hnm: %s: no symbols\n", fn);
	return (!found);
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
	int ret;

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
		fprintf(stderr, "hnm: %s: File format not recognized\n", filename);
		munmap(state.map, state.size);
		return (1);
	}
	state.is_64 = (h[EI_CLASS] == ELFCLASS64);
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
	state.swap = (h[EI_DATA] == ELFDATA2MSB);
#else
	state.swap = (h[EI_DATA] == ELFDATA2LSB);
#endif
	ret = parse_sections(&state, filename);
	munmap(state.map, state.size);
	return (ret);
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
