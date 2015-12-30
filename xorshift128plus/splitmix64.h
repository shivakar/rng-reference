#include <stdint.h>

struct SplitMix64 {
	uint64_t x;
};

typedef struct SplitMix64 splitmix64_t;

uint64_t splitmix64_uint64(splitmix64_t* random);
void splitmix64_init(splitmix64_t* random, uint64_t seed);

void splitmix64_init(splitmix64_t* random, uint64_t seed) {
	random->x = seed;
}

uint64_t splitmix64_uint64(splitmix64_t* random) {
	uint64_t z = (random->x += UINT64_C(0x9E3779B97F4A7C15));
	z = (z ^ (z >> 30)) * UINT64_C(0xBF58476D1CE4E5B9);
	z = (z ^ (z >> 27)) * UINT64_C(0x94D049BB133111EB);
	return z ^ (z >> 31);
}
