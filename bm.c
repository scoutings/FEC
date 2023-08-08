#include "bm.h"

#include "bv.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct BitMatrix {
    uint32_t rows;
    uint32_t cols;
    BitVector *vector;
};

BitMatrix *bm_create(uint32_t rows, uint32_t cols) {
    // Allocate memory
    BitMatrix *m = (BitMatrix *) malloc(sizeof(BitMatrix));
    if (m) {
        // If enough memory
        m->rows = rows;
        m->cols = cols;
        m->vector = bv_create(rows * cols);
        if (!m->vector) {
            // If was unable to allocate memory
            free(m);
            m = NULL;
        }
    }
    return m;
}

void bm_delete(BitMatrix **m) {
    if (*m && (*m)->vector) {
        bv_delete(&(*m)->vector);
        free(*m);
        *m = NULL;
    }
    return;
}

uint32_t bm_rows(BitMatrix *m) {
    return m->rows;
}

uint32_t bm_cols(BitMatrix *m) {
    return m->cols;
}

void bm_set_bit(BitMatrix *m, uint32_t r, uint32_t c) {
    if (r < m->rows && c < m->cols) {
        bv_set_bit(m->vector, (m->cols * r) + c);
    }
    return;
}

void bm_clr_bit(BitMatrix *m, uint32_t r, uint32_t c) {
    if (r < m->rows && c < m->cols) {
        bv_clr_bit(m->vector, (m->cols * r) + c);
    }
    return;
}

uint8_t bm_get_bit(BitMatrix *m, uint32_t r, uint32_t c) {
    if (r < m->rows && c < m->cols) {
        return bv_get_bit(m->vector, (m->cols * r) + c);
    } else {
        return 0;
    }
}

BitMatrix *bm_from_data(uint8_t byte, uint32_t length) {
    // Create an empty bit matrix
    BitMatrix *m = bm_create(1, length);
    for (uint32_t i = 0; i < length; i++) {
        // Loop through each bit in the byte
        if ((byte & ((uint8_t) 0x01 << (uint8_t) i % 8)) >> (uint8_t) i % 8) {
            // If the ith bit in the byte is 1, we must set it
            bm_set_bit(m, 0, i);
        }
    }
    return m;
}

uint8_t bm_to_data(BitMatrix *m) {
    uint8_t ret_val = 0;
    // Go through the first 8 bits
    for (uint8_t i = 0; i < 8; i++) {
        // we will perform bitwise operations to copy each bit at a time
        ret_val |= bv_get_bit(m->vector, i) << i;
    }
    return ret_val;
}

BitMatrix *bm_multiply(BitMatrix *A, BitMatrix *B) {
    // assert a rule of matrix multiplication
    assert(A->cols == B->rows);
    // the product matrix will be A->rows x B->cols
    BitMatrix *m = bm_create(A->rows, B->cols);
    // Loop through each row in matrix A
    for (uint32_t row = 0; row < A->rows; row++) {
        // Loop through each col in matrix B
        for (uint32_t col = 0; col < B->cols; col++) {
            uint8_t bit_val = 0;
            // Xor the product of each bit in both matrices to avoid having to do mod 2 later
            for (uint32_t count = 0; count < A->cols; count++) {
                bit_val ^= bm_get_bit(A, row, count) & bm_get_bit(B, count, col);
            }
            // If the bit value after having done multiplication is 1, we must set that bit
            if (bit_val) {
                bm_set_bit(m, row, col);
            }
        }
    }
    return m;
}

void bm_print(BitMatrix *m) {
    // debugging
    printf("Matrix: %ux%u\n__|", m->rows, m->cols);
    for (uint32_t i = 0; i < m->cols; i++) {
        printf("%2u ", i);
    }
    printf("\n");
    for (uint32_t i = 0; i < bv_length(m->vector); i++) {
        if (i % m->cols == 0) {
            printf("%2u|", i / m->cols);
        }
        printf("%2u ", bv_get_bit(m->vector, i));
        if ((i + 1) % m->cols == 0) {
            printf("\n");
        }
    }
    return;
}
