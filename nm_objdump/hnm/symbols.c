#include "hnm.h"

/**
 * print_64 - single 64-bit ELF symbol printedout
 * @st: state config struct pointer
 * @sym: pointer to the 64-bit ELF symbl
 * @n: sybl name str
 */
void print_64(elf_s_t *st, Elf64_Sym *sym, char *n)
{
	char tc;

	tc = get_type(ELF64_ST_BIND(sym->st_info),
		      ELF64_ST_TYPE(sym->st_info),
		      s2(sym->st_shndx, st->swap), st->map);
	if (tc == 'U' || tc == 'w' || tc == 'v')
		printf("                 %c %s\n", tc, n);
	else
		printf("%016lx %c %s\n",
		       (unsigned long)s8(sym->st_value, st->swap), tc, n);
}

/**
 * print_32 - single 32-bit ELF symbol printedout
 * @st: state config struct pointer
 * @sym: pointer to the 32-bit ELF symbl
 * @n: symbl name str
 */
void print_32(elf_s_t *st, Elf32_Sym *sym, char *n)
{
	char tc;

	tc = get_type(ELF32_ST_BIND(sym->st_info),
		      ELF32_ST_TYPE(sym->st_info),
		      s2(sym->st_shndx, st->swap), st->map);
	if (tc == 'U' || tc == 'w' || tc == 'v')
		printf("         %c %s\n", tc, n);
	else
		printf("%08lx %c %s\n",
		       (unsigned long)s4(sym->st_value, st->swap), tc, n);
}

/**
 * loop_syms - traverses raw sections mapping data frames sequentially
 * @st: curr file data tracking target wrapper structure tracking handle
 * @shdr: raw memory target offset base pointer address alignment metric
 * @strtab: symbl name lookup matching string array character field
 * @cnt: tot entry loop processing constraint calculation criteria limits
 * @size: architecture obj record structure sizing layout steps
 */
void loop_syms(elf_s_t *st, void *shdr, char *strtab, int cnt, int size)
{
	int i;
	char *n;

	for (i = 0; i < cnt; i++)
	{
		if (st->is_64)
		{
			Elf64_Sym *sym = (Elf64_Sym *)((char *)shdr + (i * size));

			n = strtab + s4(sym->st_name, st->swap);
			if ((!*n && s8(sym->st_value, st->swap) == 0) ||
			    (ELF64_ST_TYPE(sym->st_info) == STT_FILE) ||
			    (ELF64_ST_TYPE(sym->st_info) == STT_SECTION))
				continue;
			print_64(st, sym, n);
		}
		else
		{
			Elf32_Sym *sym = (Elf32_Sym *)((char *)shdr + (i * size));

			n = strtab + s4(sym->st_name, st->swap);
			if ((!*n && s4(sym->st_value, st->swap) == 0) ||
			    (ELF32_ST_TYPE(sym->st_info) == STT_FILE) ||
			    (ELF32_ST_TYPE(sym->st_info) == STT_SECTION))
				continue;
			print_32(st, sym, n);
		}
	}
}
