#ifndef HOBJDUMP_H
#define HOBJDUMP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <elf.h>

/**
 * struct elf_s - state tracker for target ELF binaries
 * @map: mem pointer to the raw mapped binary file bytes
 * @size: tot byte size of the mapped target binary file
 * @is_64: bool indicating architecture layout width (as 1 if 64-bit, 0 if 32)
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
void print_header(elf_s_t *st, const char *fn);
void print_sections(elf_s_t *st);

#endif
