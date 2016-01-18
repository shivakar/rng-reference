/*  Written in 2014-2015 by Sebastiano Vigna (vigna@acm.org)

To the extent possible under law, the author has dedicated all copyright
and related and neighboring rights to this software to the public domain
worldwide. This software is distributed without any warranty.

See <http://creativecommons.org/publicdomain/zero/1.0/>. */

#include <stdint.h>

/* This is the fastest generator passing BigCrush without
   systematic failures, but due to the relatively short period it is
   acceptable only for applications with a mild amount of parallelism;
   otherwise, use a xorshift1024* generator.

   The state must be seeded so that it is not everywhere zero. If you have
   a 64-bit seed, we suggest to seed a splitmix64 generator and use its
   output to fill s. */

uint64_t s[2];
uint64_t uint64(void);
double float64(void);
double float64oo(void);

uint64_t uint64(void) {
	uint64_t s1 = s[0];
	const uint64_t s0 = s[1];
	s[0] = s0;
	s1 ^= s1 << 23; // a
	s[1] = s1 ^ s0 ^ (s1 >> 18) ^ (s0 >> 5); // b, c
	return s[1] + s0;
}

/* Return double in [0.0, 1.0) */
double float64() {
    return ((double)(uint64() >> 11)) / ((double)(((uint64_t)1) <<53));
}

/* Return double in (0.0, 1.0) */
double float64oo() {
    return ((double)(uint64() >> 12) + 0.5) / ((double)(((uint64_t)1) <<52));
}
