#include "hobjdump.h"

/**
 * s2 - swaps bytes for a 16-bit wide unsigned integer
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
