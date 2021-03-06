/*  Written in 2015 by Sebastiano Vigna (vigna@acm.org)

To the extent possible under law, the author has dedicated all copyright
and related and neighboring rights to this software to the public domain
worldwide. This software is distributed without any warranty.

See <http://creativecommons.org/publicdomain/zero/1.0/>. */

#include <stdint.h>

/* This is a fixed-increment version of Java 8's SplittableRandom generator
   See http://dx.doi.org/10.1145/2714064.2660195 and
   http://docs.oracle.com/javase/8/docs/api/java/util/SplittableRandom.html

   It is a very fast generator passing BigCrush, and it can be useful if
   for some reason you absolutely want 64 bits of state; otherwise, we
   rather suggest to use a xorshift128+ (for moderately parallel
   computations) or xorshift1024* (for massively parallel computations)
   generator. */

uint64_t x; /* The state can be seeded with any value. */
uint64_t uint64(void);
double float64(void);
double float64oo(void);

uint64_t uint64() {
	uint64_t z = (x += UINT64_C(0x9E3779B97F4A7C15));
	z = (z ^ (z >> 30)) * UINT64_C(0xBF58476D1CE4E5B9);
	z = (z ^ (z >> 27)) * UINT64_C(0x94D049BB133111EB);
	return z ^ (z >> 31);
}

/* Return double in [0.0, 1.0) */
double float64() {
    return ((double)(uint64() >> 11)) / ((double)(((uint64_t)1) <<53));
}

/* Return double in (0.0, 1.0) */
double float64oo() {
    return ((double)(uint64() >> 12) + 0.5) / ((double)(((uint64_t)1) <<52));
}
