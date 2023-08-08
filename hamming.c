#include "hamming.h"

#include <stdio.h>

static HAM_STATUS look_up_table[16] = { HAM_OK, 4, 5, HAM_ERR, 6, HAM_ERR, HAM_ERR, 3, 7, HAM_ERR,
    HAM_ERR, 2, HAM_ERR, 1, 0, HAM_ERR };

uint8_t ham_encode(BitMatrix *G, uint8_t msg) {
    // Will return this value after freeing all allocs
    uint8_t ret_val;
    // Create the lower nibble matrix, and multiply
    BitMatrix *m = bm_from_data(msg, 4);
    BitMatrix *c = bm_multiply(m, G);
    // Our return value is the first 8 bits of the product
    ret_val = bm_to_data(c);
    bm_delete(&m);
    bm_delete(&c);
    return ret_val;
}

HAM_STATUS ham_decode(BitMatrix *Ht, uint8_t code, uint8_t *msg) {
    // This will be the value we return
    HAM_STATUS ret_val;
    // Create a matrix out of the code
    BitMatrix *c = bm_from_data(code, 8);
    // Multiply the matrices
    BitMatrix *e = bm_multiply(c, Ht);
    // Next we get the value of the product
    uint8_t decode = bm_to_data(e);
    // Compare this with the lookup table
    // If it has detected error
    if (look_up_table[decode] != HAM_OK) {
        // is it unfixable?
        if (look_up_table[decode] == HAM_ERR) {
            ret_val = HAM_ERR;
        } else {
            // Flip the bit
            ret_val = HAM_CORRECT;
            if (bm_get_bit(c, 0, look_up_table[decode])) {
                bm_clr_bit(c, 0, look_up_table[decode]);
            } else {
                bm_set_bit(c, 0, look_up_table[decode]);
            }
        }
    } else {
        // No errors means it is ok
        ret_val = HAM_OK;
    }
    if (look_up_table[decode] != HAM_ERR) {
        *msg = 0;
        for (int i = 0; i < 4; i++) {
            // message are the first four bits
            *msg |= bm_get_bit(c, 0, i) << i;
        }
    }
    // free any bit matrices used
    bm_delete(&c);
    bm_delete(&e);
    return ret_val;
}
