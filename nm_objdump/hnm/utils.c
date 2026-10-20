#include "hnm.h"

/**
 * s2 - conditionally swaps bytes for a 16-bit wide unsigned integer
 * @v: val to process
 * @swp: byte swapping execution boolean conditional toggle
 * Return: swapped or native unsigned integer value content
 */
uint16_t s2(uint16_t v, int swp)
{
	return (swp ? (uint16_t)((v >> 8) | (v << 8)) : v);
}

/**
 * s4 - conditionally swaps bytes for a 32-bit wide unsigned integer
 * @v: val to process
 * @swp: byte swapping execution boolean conditional toggle
 * Return: swapped or native unsigned integer value content
 */
uint32_t s4(uint32_t v, int swp)
{
	if (!swp)
		return (v);
	return (((v & 0xff000000) >> 24) | ((v & 0x00ff0000) >> 8) |
			((v & 0x0000ff00) << 8) | ((v & 0x000000ff) << 24));
}

/**
 * s8 - conditionally swaps bytes for a 64-bit wide unsigned integer
 * @v: val to process
 * @swp: byte swapping execution boolean conditional toggle
 * Return: swapped or native unsigned integer value content
 */
uint64_t s8(uint64_t v, int swp)
{
	if (!swp)
		return (v);
	return (((v & 0xff00000000000000ULL) >> 56) |
			((v & 0x00ff000000000000ULL) >> 40) |
			((v & 0x0000ff0000000000ULL) >> 24) |
			((v & 0x000000ff00000000ULL) >> 8) |
			((v & 0x00000000ff000000ULL) << 8) |
			((v & 0x0000000000ff0000ULL) << 24) |
			((v & 0x000000000000ff00ULL) << 40) |
			((v & 0x00000000000000ffULL) << 56));
}

/**
 * get_type - resolves NM code descriptors based on section attributes
 * @b: ELF symbl binding property value parameter
 * @t: ELF symbl type structural description attribute type parameter
 * @idx: segment index pointer location map context offset metrics
 * @m: file map structural pointer anchor metric map context
 * Return: ASCII char symbol context flag character mapping
 */
char get_type(unsigned char b, unsigned char t, unsigned short idx, void *m)
{
	char c = 'D';
	(void)m;

	if (b == STB_WEAK)
	{
		c = (t == STT_OBJECT) ? 'V' : 'W';
		if (idx == SHN_UNDEF)
			c = (t == STT_OBJECT) ? 'v' : 'w';
		return (c);
	}
	if (idx == SHN_UNDEF)
		return ('U');
	if (idx == SHN_ABS)
		c = 'A';
	else if (idx == SHN_COMMON)
		c = 'C';
	else if (t == STT_FUNC)
		c = 'T';

	if (b == STB_LOCAL && c != '?')
		c += 32;
	return (c);
}
