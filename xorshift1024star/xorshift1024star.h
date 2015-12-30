/*  Written in 2014-2015 by Sebastiano Vigna (vigna@acm.org)

To the extent possible under law, the author has dedicated all copyright
and related and neighboring rights to this software to the public domain
worldwide. This software is distributed without any warranty.

See <http://creativecommons.org/publicdomain/zero/1.0/>. */

#include <stdint.h>

/* This is a fast, top-quality generator. If 1024 bits of state are too
   much, try a xorshift128+ generator.

   The state must be seeded so that it is not everywhere zero. If you have
   a 64-bit seed, we suggest to seed a splitmix64 generator and use its
   output to fill s. */

uint64_t s[16];
int p;
const double mul = 1.0 / 18446744073709551616.0;

uint64_t xorshift1024star_uint64(void);
double xorshift1024star_float64(void);

uint64_t xorshift1024star_uint64(void) {
	const uint64_t s0 = s[p];
	uint64_t s1 = s[p = (p + 1) & 15];
	s1 ^= s1 << 31; // a
	s[p] = s1 ^ s0 ^ (s1 >> 11) ^ (s0 >> 30); // b,c
	return s[p] * UINT64_C(1181783497276652981);
}

double xorshift1024star_float64() {
	return xorshift1024star_uint64() * mul;
}
