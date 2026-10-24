#include "hobjdump.h"

/**
 * print_hex_row - helper to dump a raw line of content bytes
 * @sect: starting pointer position of section
 * @addr: mem address translation
 * @i: curr absolute loop row offset position
 * @sz: max tracking section boundaries size
 */
void print_hex_row(unsigned char *sect, uint64_t addr, uint64_t i, uint64_t sz)
{
	uint64_t j;

	printf(" %04lx ", (unsigned long)(addr + i));
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
 */
void dump_section(const char *name, void *sect, uint64_t addr, uint64_t sz)
{
	uint64_t i;

	printf("Contents of section %s:\n", name);
	for (i = 0; i < sz; i += 16)
	{
		print_hex_row(sect, addr, i, sz);
	}
}

/**
 * print_sections - loop through sections extracting allocatable frames
 * @st: curr architecture structure metadata tracking wrapper layout
 */
void print_sections(elf_s_t *st)
{
	int i;
	char *shstr, *name;

	if (st->is_64)
	{
		Elf64_Ehdr *eh = st->map;
		Elf64_Shdr *sh = (Elf64_Shdr *)((char *)st->map + s8(eh->e_shoff, st->swap));

		shstr = (char *)st->map + s8(sh[s2(eh->e_shstrndx, st->swap)].sh_offset, st->swap);
		for (i = 0; i < s2(eh->e_shnum, st->swap); i++)
		{
			name = shstr + s4(sh[i].sh_name, st->swap);
			if (s4(sh[i].sh_type, st->swap) == SHT_NOBITS ||
			    !(s8(sh[i].sh_flags, st->swap) & SHF_ALLOC) ||
			    s8(sh[i].sh_size, st->swap) == 0 ||
			    *name == '\0' || strcmp(name, ".strtab") == 0 ||
			    strcmp(name, ".symtab") == 0 || strcmp(name, ".shstrtab") == 0)
				continue;
			dump_section(name, (char *)st->map + s8(sh[i].sh_offset, st->swap),
				     s8(sh[i].sh_addr, st->swap), s8(sh[i].sh_size, st->swap));
		}
	}
	else
	{
		Elf32_Ehdr *eh = st->map;
		Elf32_Shdr *sh = (Elf32_Shdr *)((char *)st->map + s4(eh->e_shoff, st->swap));

		shstr = (char *)st->map + s4(sh[s2(eh->e_shstrndx, st->swap)].sh_offset, st->swap);
		for (i = 0; i < s2(eh->e_shnum, st->swap); i++)
		{
			name = shstr + s4(sh[i].sh_name, st->swap);
			if (s4(sh[i].sh_type, st->swap) == SHT_NOBITS ||
			    !(s4(sh[i].sh_flags, st->swap) & SHF_ALLOC) ||
			    s4(sh[i].sh_size, st->swap) == 0 ||
			    *name == '\0' || strcmp(name, ".strtab") == 0 ||
			    strcmp(name, ".symtab") == 0 || strcmp(name, ".shstrtab") == 0)
				continue;
			dump_section(name, (char *)st->map + s4(sh[i].sh_offset, st->swap),
				     s4(sh[i].sh_addr, st->swap), s4(sh[i].sh_size, st->swap));
		}
	}
}
