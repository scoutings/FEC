#include "bv.h"

#include <stdio.h>
#include <stdlib.h>

struct BitVector {
    uint32_t length;
    uint8_t *vector;
};

BitVector *bv_create(uint32_t length) {
    // Allocate memory
    BitVector *v = (BitVector *) malloc(sizeof(BitVector));
    if (v) {
        // If enough memory
        v->length = length;
        v->vector = (uint8_t *) calloc((length / 8 + (length % 8 != 0)), sizeof(uint8_t));
        if (!v->vector) {
            // If was unable to allocate memory
            free(v);
            v = NULL;
        }
    }
    return v;
}

void bv_delete(BitVector **v) {
    if (*v && (*v)->vector) {
        free((*v)->vector);
        free(*v);
        *v = NULL;
    }
    return;
}

uint32_t bv_length(BitVector *v) {
    return v->length;
}

void bv_set_bit(BitVector *v, uint32_t i) {
    if (i < v->length) {
        v->vector[i / 8] |= ((uint8_t) 0x01 << (uint8_t) i % 8);
    }
    return;
}

void bv_clr_bit(BitVector *v, uint32_t i) {
    if (i < v->length) {
        v->vector[i / 8] &= ~((uint8_t) 0x01 << (uint8_t) i % 8);
    }
    return;
}

void bv_xor_bit(BitVector *v, uint32_t i, uint8_t bit) {
    if (i < v->length) {
        v->vector[i / 8] ^= (bit << (uint8_t) i % 8);
    }
    return;
}

uint8_t bv_get_bit(BitVector *v, uint32_t i) {
    if (i < v->length) {
        return (v->vector[i / 8] & ((uint8_t) 0x01 << (uint8_t) i % 8)) >> (uint8_t) i % 8;
    } else {
        return 0;
    }
}

void bv_print(BitVector *v) {
    // Debugging function
    printf("Bit Vector length: %u Bits\n", v->length);
    for (uint32_t i = 0; i < v->length; i++) {
        printf("%2u ", bv_get_bit(v, i));
    }
    printf("\n");
    for (uint32_t i = 0; i < v->length; i++) {
        printf("%2u ", i);
    }
    printf("\n");
    return;
}
