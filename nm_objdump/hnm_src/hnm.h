#ifndef HNM_H
#define HNM_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <elf.h>

/**
 * struct elf_s - state tracker for target ELF binaries
 * @map: mem pointer to the raw mapped binary file bytes
 * @size: tot byte size of the mapped target binary file
 * @is_64: bool indicating architecture layout width (1 if 64-bit, 0 if 32)
 * @swap: bool indicating endian byte swapping necessity flags
 */
typedef struct elf_s
{
	void *map;
	size_t size;
	int is_64;
	int swap;
} elf_s_t;

uint16_t s2(uint16_t v, int swp);
uint32_t s4(uint32_t v, int swp);
uint64_t s8(uint64_t v, int swp);

char get_type(unsigned char b, unsigned char t, unsigned short idx,
	      uint64_t shflags, uint32_t shtype);
void loop_syms(elf_s_t *st, void *shdr, char *strtab, int cnt, int size,
	       void *sh);

void print_64(elf_s_t *st, Elf64_Sym *sym, char *n, Elf64_Shdr *sh);
void print_32(elf_s_t *st, Elf32_Sym *sym, char *n, Elf32_Shdr *sh);

#endif /* HNM_H */
