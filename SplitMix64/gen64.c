#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include "splitmix64.h"

typedef enum {UINT64, DOUBLE_OO, DOUBLE_CO} result_type;

typedef uint64_t (*generator_int) ();
typedef double (*generator_double) ();

void reset(uint64_t);
void check_file(FILE*);
void write_samples_to_file(uint64_t, uint64_t, uint64_t, result_type);

void usage(const char*, int);

void usage(const char* progName, int exit_val) {
        fprintf(stderr, "%s [-h] -s seed -t startIdx -n numSamples", progName);
        fprintf(stderr, "\nwhere options are:");
        fprintf(stderr, "\n    -s    : Seed for the RNG engine");
        fprintf(stderr, "\n    -t    : Starting index (starting with 0) of RNG draws to output");
        fprintf(stderr, "\n    -n    : Number of pseudo-random samples to output (default 25)");
        fprintf(stderr, "\n    -h    : Show this help");
        fprintf(stderr, "\n\nIf no options are provided, files for the following seeds and startIdx");
        fprintf(stderr, "\n            combinations are generated:");
        fprintf(stderr, "\n    uint64_t seeds[5] = { 1, 1740, 3812201, 6662751973, 5164531120897553273};");
        fprintf(stderr, "\n    uint64_t starts[7] = { 0, 1024, 9118, 674637, 7536456, 1e9, 5e9 };");
        fprintf(stderr, "\n\nOutput filenames use the following naming convention:");
        fprintf(stderr, "\n    tinymt64-<seed>-<sample_type>-<start_idx>-<num_samples>.txt");
        fprintf(stderr, "\nWhere 'sample_type' values indicate the following RNG output ranges:");
        fprintf(stderr, "\n    'uint64': [0, 2^64)");
        fprintf(stderr, "\n    'double_co': [0.0, 1.0)");
        fprintf(stderr, "\n    'double_oc': (0.0, 1.0]");
        fprintf(stderr, "\n    'double_oo': (0.0, 1.0)");
        fprintf(stderr, "\n");
        fflush(stderr);
        exit(exit_val);
}

void reset(uint64_t seed) {
        x = seed;
}

void check_file(FILE *fp) {
        if (fp == NULL) {
                fprintf(stderr, "Can't open file\n");
                exit(1);
        }
}

void write_samples_to_file(uint64_t seed, uint64_t num_samples,
                           uint64_t first_idx, result_type rt) {

        char rt_str[15];
        reset(seed);

        generator_int igen = NULL;
        generator_double dgen = NULL;

        switch (rt) {
        case UINT64:
                igen = uint64;
                sprintf(rt_str, "uint64");
                break;
        case DOUBLE_CO:
                dgen = float64;
                sprintf(rt_str, "double_co");
                break;
        case DOUBLE_OO:
                dgen = float64oo;
                sprintf(rt_str, "double_oo");
                break;
        }


        // Create filename
        char filename[255];
        sprintf(filename, "splitmix64-%" PRIu64 "-%s-%" PRIu64 "-%" PRIu64 ".txt",
                seed, rt_str, first_idx, num_samples);
        FILE* fp = fopen(filename, "w");
        check_file(fp);
        fprintf(stderr, "Writing to %s\n", filename);



        // fprintf(fp, "%"PRIu64"\n", seed); // Write seed as the first line
        // Skip till first_idx
        if (rt == UINT64) {
                for (uint64_t i = 0; i < first_idx; i++) {
                        igen();
                }
                for (uint64_t i = 0; i < num_samples; i++) {
                        fprintf(fp, "%" PRIu64 "\n", igen());
                }
        } else {
                for (uint64_t i = 0; i < first_idx; i++) {
                        dgen();
                }
                for (uint64_t i = 0; i < num_samples; i++) {
                        fprintf(fp, "%.17f\n", dgen());
                }
        }

        fclose(fp);
}

int main(int argc, char **argv) {
        uint64_t seeds[5] = { 1, 1740, 3812201, 6662751973, 5164531120897553273};
        uint64_t starts[7] = { 0, 1024, 9118, 674637, 7536456, 1e9, 5e9 };
        uint64_t num_seeds = 5;
        uint64_t num_starts = 7;
        uint64_t num_samples = 25;

        // Parse command line
        char o; // option
        for (; (o=getopt(argc, argv, "hn:s:t:")) != -1; ) {
            switch(o) {
                case 'h':
                    usage(argv[0], 0);
                    break;
                case 's':
                    seeds[0] = atol(optarg);
                    num_seeds = 1;
                    break;
                case 't':
                    starts[0] = atol(optarg);
                    num_starts = 1;
                    break;
                case 'n':
                    num_samples = atol(optarg);
                    break;
                case '?':
                    usage(argv[0], 10);
                    break;
                default:
                    usage(argv[0], 10);
                    break;
            }
        }

        for (int i = 0; i < num_seeds; i++) {
                for (int j = 0; j < num_starts; j++) {
                        for (int rt = UINT64; rt <= DOUBLE_CO; ++rt) {
                                write_samples_to_file(seeds[i], num_samples,
                                                      starts[j], rt);
                        }
                }
        }

        return 0;
}
