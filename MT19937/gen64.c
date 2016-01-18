#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include "mt64.h"


typedef unsigned long long int_type;

typedef enum {UINT64, DOUBLE_CO, DOUBLE_OO, DOUBLE_OC} result_type;

typedef int_type (*generator_int) (void);
typedef double (*generator_double) (void);


void check_file(FILE*);
void write_samples_to_file(int_type, int_type, int_type, result_type);

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
        fprintf(stderr, "\n    int_type seeds[5] = { 1, 1740, 3812201, 6662751973, 5164531120897553273};");
        fprintf(stderr, "\n    int_type starts[7] = { 0, 1024, 9118, 674637, 7536456, 1e9, 5e9 };");
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

void check_file(FILE *fp) {
        if (fp == NULL) {
                fprintf(stderr, "Can't open file\n");
                exit(1);
        }
}

void write_samples_to_file(int_type seed, int_type num_samples,
                           int_type first_idx, result_type rt) {

        char rt_str[15];
        init_genrand64(seed);

        generator_int igen = NULL;
        generator_double dgen = NULL;

        switch (rt) {
        case UINT64:
                igen = genrand64_int64;
                sprintf(rt_str, "uint64");
                break;
        case DOUBLE_CO:
                dgen = genrand64_real2;
                sprintf(rt_str, "double_co");
                break;
        case DOUBLE_OC:
                dgen = genrand64_real4;
                sprintf(rt_str, "double_oc");
                break;
        case DOUBLE_OO:
                dgen = genrand64_real3;
                sprintf(rt_str, "double_oo");
                break;
        }


        // Create filename
        char filename[255];
        sprintf(filename, "mt19937-%" PRIu64 "-%s-%" PRIu64 "-%" PRIu64 ".txt",
                seed, rt_str, first_idx, num_samples);
        FILE* fp = fopen(filename, "w");
        check_file(fp);
        fprintf(stderr, "Writing to %s\n", filename);



        // fprintf(fp, "%"PRIu64"\n", seed); // Write seed as the first line
        // Skip till first_idx
        if (rt == UINT64) {
                for (int_type i = 0; i < first_idx; i++) {
                        igen();
                }
                for (int_type i = 0; i < num_samples; i++) {
                        fprintf(fp, "%" PRIu64 "\n", igen());
                }
        } else {
                for (int_type i = 0; i < first_idx; i++) {
                        dgen();
                }
                for (int_type i = 0; i < num_samples; i++) {
                        fprintf(fp, "%.17f\n", dgen());
                }
        }

        fclose(fp);
}

int main(int argc, char **argv) {
        int_type seeds[5] = { 1, 1740, 3812201, 6662751973, 5164531120897553273};
        int_type starts[7] = { 0, 1024, 9118, 674637, 7536456, 1e9, 5e9 };
        int_type num_seeds = 5;
        int_type num_starts = 7;
        int_type num_samples = 25;

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
                        for (int rt = UINT64; rt <= DOUBLE_OC; ++rt) {
                                write_samples_to_file(seeds[i], num_samples,
                                                      starts[j], rt);
                        }
                }
        }

        return 0;
}
