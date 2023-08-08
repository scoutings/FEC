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

#define OPTIONS "hi:o:"

static void print_helper() {
    printf("SYNOPSIS\n  A Hamming(8, 4) systematic code generator.\n\nUSAGE\n  ./encode [-h] [-i "
           "infile] [-o outfile]\n\nOPTIONS\n  -h             Program usage and help.\n  -i infile "
           "     Input data to encode.\n  -o outfile     Output of encoded data.\n");
    return;
}

// returns the lower nibble of val (this was taken from the design doc)
static uint8_t lower_nibble(uint8_t val) {
    return val & 0xF;
}

// returns the upper nibble of val (this was taken from the design doc)
static uint8_t upper_nibble(uint8_t val) {
    return val >> 4;
}

int main(int argc, char **argv) {
    int opt;
    // By default our input output files are stdin/stdout
    enum { INPUT, OUTPUT };
    // This will be used to deteremine whether we need to close a file or not
    bool options[2] = { false, false };
    FILE *infile = stdin;
    FILE *outfile = stdout;

    // Parsing program arguments
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h': print_helper(); exit(0);
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

    BitMatrix *G = bm_create_G();

    while (true) {
        int8_t byte = fgetc(infile);

        if (byte == EOF) {
            break;
        }

        uint8_t lower_byte = lower_nibble(byte);
        uint8_t upper_byte = upper_nibble(byte);

        fputc(ham_encode(G, lower_byte), outfile);
        fputc(ham_encode(G, upper_byte), outfile);
    }

    bm_delete(&G);
    if (options[INPUT]) {
        fclose(infile);
    }
    if (options[OUTPUT]) {
        fclose(outfile);
    }
    return 0;
}
