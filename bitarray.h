#ifndef BITARRAY_H //aby se to nedelalo vicekrat kdyz uz to existuje a nerval po me compiler
#define BITARRAY_H

#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <stdint.h>


typedef unsigned long bitarray_t[];
typedef unsigned long bitarray_index_t;

//pomocne makrko na vzorec
#define BITARRAY_BLOCKS(velikost) (((velikost) + (sizeof(unsigned long) * CHAR_BIT) - 1) / (sizeof(unsigned long) * CHAR_BIT) + 1)

#define bitarray_create(jmeno_pole, velikost) \
    unsigned long jmeno_pole[BITARRAY_BLOCKS(velikost)] = {(velikost), 0};\
    static_assert((velikost) > 0, "Velikost pole musi byt kladna")
//celociselne deleni musi zaokrouhlit nahoru, ne dolu a pridame jednu bunku pro 0 z toho vychazi vzorec

#define bitarray_alloc(jmeno_pole, velikost) \
    unsigned long *jmeno_pole = calloc(BITARRAY_BLOCKS(velikost),sizeof(unsigned long));\
    assert((velikost) > 0 && (velikost) <= (ULONG_MAX - (sizeof(unsigned long) * CHAR_BIT)));\
    if(jmeno_pole == NULL) { \
        fprintf(stderr, "bitarray_alloc: Chyba alokace pameti\n"); \
        exit(1);\
    }\
    jmeno_pole[0] = (velikost)

#define bitarray_free(jmeno_pole)


#endif
