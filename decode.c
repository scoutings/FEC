#include "bm.h"
#include "bv.h"
#include "constants.h"
#include "hamming.h"

#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define OPTIONS "hvi:o:"

static void print_helper() {
    printf("SYNOPSIS\n  A Hamming(8, 4) systematic code decoder.\n\nUSAGE\n  ./decode [-h] [-v] "
           "[-i infile] [-o outfile]\n\nOPTIONS\n  -h             Program usage and help.\n  -v    "
           "         Print statistics of decoding to stderr.\n  -i infile "
           "     Input data to encode.\n  -o outfile     Output of encoded data.\n");
    return;
}

static void print_stats(
    uint64_t total_bytes_processed, uint64_t uncorrected_errors, uint64_t corrected_errors) {
    fprintf(stderr,
        "Total bytes processed: %lu\nUncorrected errors: %lu\nCorrected errors: %lu\nError "
        "rate: %f\n",
        total_bytes_processed, uncorrected_errors, corrected_errors,
        ((double) uncorrected_errors / (double) total_bytes_processed));
    return;
}

// Packs two nibbles into a byte
static uint8_t pack_byte(uint8_t upper, uint8_t lower) {
    return (upper << 4) | (lower & 0x0F);
}

int main(int argc, char **argv) {
    int opt;
    // By default our input output files are stdin/stdout
    enum { VERBOSE, INPUT, OUTPUT };
    // This will be used to deteremine whether we need to close a file or not
    bool options[3] = { false, false, false };
    FILE *infile = stdin;
    FILE *outfile = stdout;

    // Parsing program arguments
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h': print_helper(); exit(0);
        case 'v': options[VERBOSE] = true; break;
        case 'i':
            options[INPUT] = true;
            infile = fopen(optarg, "rb");
            break;
        case 'o':
            options[OUTPUT] = true;
            outfile = fopen(optarg, "wb");
            break;
        }
    }

    if (options[INPUT] && options[OUTPUT]) {
        struct stat statbuf;
        fstat(fileno(infile), &statbuf);
        fchmod(fileno(outfile), statbuf.st_mode);
    }

    BitMatrix *Ht = bm_create_Ht();

    uint64_t total_bytes_processed = 0;
    uint64_t uncorrected_errors = 0;
    uint64_t corrected_errors = 0;

    uint8_t lower_nibble = 0;
    uint8_t upper_nibble = 0;

    while (true) {
        int8_t lower_byte = fgetc(infile);
        int8_t upper_byte = fgetc(infile);

        if (upper_byte == EOF) {
            break;
        }

        HAM_STATUS lower = ham_decode(Ht, lower_byte, &lower_nibble);
        HAM_STATUS upper = ham_decode(Ht, upper_byte, &upper_nibble);

        if (options[VERBOSE]) {
            total_bytes_processed += 2;
            if (lower == HAM_CORRECT) {
                corrected_errors += 1;
            }
            if (lower == HAM_ERR) {
                uncorrected_errors += 1;
            }
            if (upper == HAM_CORRECT) {
                corrected_errors += 1;
            }
            if (upper == HAM_ERR) {
                uncorrected_errors += 1;
            }
        }

        fputc(pack_byte(upper_nibble, lower_nibble), outfile);
    }

    if (options[VERBOSE]) {
        print_stats(total_bytes_processed, uncorrected_errors, corrected_errors);
    }

    bm_delete(&Ht);
    if (options[INPUT]) {
        fclose(infile);
    }
    if (options[OUTPUT]) {
        fclose(outfile);
    }
    return 0;
}
