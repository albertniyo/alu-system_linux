#include "hobjdump.h"

/**
 * print_hex_row - helper to dump a raw line of content bytes
 * @sect: starting pointer position of section
 * @addr: mem address translation
 * @i: curr absolute loop row offset position
 * @sz: max tracking section boundaries size
 * @is_64: architecture bit width indicator
 */
void print_hex_row(unsigned char *sect, uint64_t addr, uint64_t i,
		   uint64_t sz, int is_64)
{
	uint64_t j;

	if (is_64)
		printf(" %016lx ", (unsigned long)(addr + i));
	else
		printf(" %08lx ", (unsigned long)(addr + i));
	for (j = 0; j < 16; j++)
	{
		if (i + j < sz)
			printf("%02x", sect[i + j]);
		else
			printf("  ");
		if ((j + 1) % 4 == 0)
			printf(" ");
	}
	printf(" ");
	for (j = 0; j < 16; j++)
	{
		if (i + j < sz)
		{
			unsigned char c = sect[i + j];

			printf("%c", (c >= 32 && c <= 126) ? c : '.');
		}
		else
			printf(" ");
	}
	printf("\n");
}

/**
 * dump_section - wrapper to output header context before content
 * @name: section tracking name string
 * @sect: raw payload source byte context address pointers
 * @addr: translated file entry address boundaries
 * @sz: tot active footprint data widths metric count sizing limits
 * @is_64: architecture bit width indicator
 */
void dump_section(const char *name, void *sect, uint64_t addr,
		  uint64_t sz, int is_64)
{
	uint64_t i;

	printf("Contents of section %s:\n", name);
	for (i = 0; i < sz; i += 16)
	{
		print_hex_row(sect, addr, i, sz, is_64);
	}
}

/**
 * print_sections_64 - loop through sections for 64-bit ELF binaries
 * @st: curr architecture structure metadata tracking wrapper layout
 */
static void print_sections_64(elf_s_t *st)
{
	int i;
	uint16_t shstrndx;
	char *shstr, *name;
	Elf64_Ehdr *eh = st->map;
	Elf64_Shdr *sh = (Elf64_Shdr *)((char *)st->map +
					s8(eh->e_shoff, st->swap));

	shstrndx = s2(eh->e_shstrndx, st->swap);
	shstr = (char *)st->map + s8(sh[shstrndx].sh_offset, st->swap);
	for (i = 0; i < s2(eh->e_shnum, st->swap); i++)
	{
		uint32_t type = s4(sh[i].sh_type, st->swap);
		uint64_t flags = s8(sh[i].sh_flags, st->swap);
		uint64_t size = s8(sh[i].sh_size, st->swap);
		uint64_t offset = s8(sh[i].sh_offset, st->swap);
		uint64_t addr = s8(sh[i].sh_addr, st->swap);

		name = shstr + s4(sh[i].sh_name, st->swap);
		if (type == SHT_NOBITS || !(flags & SHF_ALLOC) ||
		    size == 0 || *name == '\0')
			continue;
		dump_section(name, (char *)st->map + offset,
			     addr, size, st->is_64);
	}
}

/**
 * print_sections_32 - loop through sections for 32-bit ELF binaries
 * @st: curr architecture structure metadata tracking wrapper layout
 */
static void print_sections_32(elf_s_t *st)
{
	int i;
	uint16_t shstrndx;
	char *shstr, *name;
	Elf32_Ehdr *eh = st->map;
	Elf32_Shdr *sh = (Elf32_Shdr *)((char *)st->map +
					s4(eh->e_shoff, st->swap));

	shstrndx = s2(eh->e_shstrndx, st->swap);
	shstr = (char *)st->map + s4(sh[shstrndx].sh_offset, st->swap);
	for (i = 0; i < s2(eh->e_shnum, st->swap); i++)
	{
		uint32_t type = s4(sh[i].sh_type, st->swap);
		uint32_t flags = s4(sh[i].sh_flags, st->swap);
		uint32_t size = s4(sh[i].sh_size, st->swap);
		uint32_t offset = s4(sh[i].sh_offset, st->swap);
		uint32_t addr = s4(sh[i].sh_addr, st->swap);

		name = shstr + s4(sh[i].sh_name, st->swap);
		if (type == SHT_NOBITS || !(flags & SHF_ALLOC) ||
		    size == 0 || *name == '\0')
			continue;
		dump_section(name, (char *)st->map + offset,
			     addr, size, st->is_64);
	}
}

/**
 * print_sections - loop through sections extracting allocatable frames
 * @st: curr architecture structure metadata tracking wrapper layout
 */
void print_sections(elf_s_t *st)
{
	if (st->is_64)
		print_sections_64(st);
	else
		print_sections_32(st);
}
