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


//pomocne makrko na vypocet velikosti pole
#define BITARRAY_BLOCKS(velikost) (((velikost) + (sizeof(unsigned long) * CHAR_BIT) - 1) / (sizeof(unsigned long) * CHAR_BIT) + 1)

//create
#define bitarray_create(jmeno_pole, velikost) \
    unsigned long jmeno_pole[BITARRAY_BLOCKS(velikost)] = {(velikost), 0};\
    static_assert((velikost) > 0, "Velikost pole musi byt kladna")
//celociselne deleni musi zaokrouhlit nahoru, ne dolu a pridame jednu bunku pro 0 z toho vychazi vzorec

//alloc
#define bitarray_alloc(jmeno_pole, velikost) \
    unsigned long *jmeno_pole = calloc(BITARRAY_BLOCKS(velikost),sizeof(unsigned long));\
    assert((velikost) > 0 && (velikost) <= (ULONG_MAX - (sizeof(unsigned long) * CHAR_BIT)));\
    if(jmeno_pole == NULL) { \
        fprintf(stderr, "bitarray_alloc: Chyba alokace pameti\n"); \
        exit(1);\
    }\
    jmeno_pole[0] = (velikost)

//free
#define bitarray_free(jmeno_pole) free(jmeno_pole)

#ifdef USE_INLINE

inline unsigned long bitarray_size(bitarray_t jmeno_pole){
    return jmeno_pole[0];
    }
inline void bitarray_fill(bitarray_t jmeno_pole, int bool_vyraz) {
    unsigned long pocet_bunek = BITARRAY_BLOCKS(bitarray_size(jmeno_pole));
    for(unsigned long i = 1; i<=pocet_bunek; i++) {
        if(bool_vyraz) {
            jmeno_pole[i] = ULONG_MAX;
        }
        else {
            jmeo_pole[i] = 0UL;
        }
    }
}

inline void

#else

//size
#define bitarray_size(jmeno_pole) jmeno_pole[0]

//fill
#define bitarray_fill(jmeno_pole, bool_vyraz) \
    do { \
        unsigned long _pocet_bunek = BITARRAY_BLOCKS(bitarray_size(jmeno_pole)); \
        for (unsigned long _i = 1; _i <= _pocet_bunek ;_i++){ \
            jmeno_pole[_i] = (bool_vyraz) ? ULONG_MAX : 0UL \
        }\
    } while(0)

//nastavi bit na 0 nebo 1, check, no_check
// Nastaví bit na zadaném indexu na hodnotu bool_vyraz (0=false, cokoliv jineho=true)
#ifndef NO_CHECK
    #define bitarray_setbit(jmeno_pole, index, bool_vyraz) \
        do { \
            if ((index) >= bitarray_size(jmeno_pole)) { \
                error_exit("bitarray_setbit: Index %lu mimo rozsah 0..%lu", \
                           (unsigned long)(index), \
                           (unsigned long)(bitarray_size(jmeno_pole) - 1)); \
            } \
            if (bool_vyraz) { \
                jmeno_pole[((index) / (sizeof(unsigned long) * CHAR_BIT)) + 1] |= \
                (1UL << ((index) % (sizeof(unsigned long) * CHAR_BIT))); \
            } else { \
                jmeno_pole[((index) / (sizeof(unsigned long) * CHAR_BIT)) + 1] &= \
                ~(1UL << ((index) % (sizeof(unsigned long) * CHAR_BIT))); \
            } \
        } while(0)
#else
    #define bitarray_setbit(jmeno_pole, index, bool_vyraz) \
        do { \
            if (bool_vyraz) { \
                jmeno_pole[((index) / (sizeof(unsigned long) * CHAR_BIT)) + 1] |= \
                (1UL << ((index) % (sizeof(unsigned long) * CHAR_BIT))); \
            } else { \
                jmeno_pole[((index) / (sizeof(unsigned long) * CHAR_BIT)) + 1] &= \
                ~(1UL << ((index) % (sizeof(unsigned long) * CHAR_BIT))); \
            } \
        } while(0)
#endif

//vrati hodontu bitu check, no_check
#ifndef NO_CHECK

//musime pouzit operator carka
    #define bitarray_getbit(jmeno_pole, index) \
        (index >= bitarray_size(jmeno_pole)) ? \
        (error_exit("bitarray_getbit: Index %lu mimo rozsah 0..%lu",(unsigned long)(index),\
        (unsigned long) (bitarray_size(jmeno_pole)-1)),0): 0 \
        (jmeno_pole[((index)/(sizeof(unsigned long) * CHAR_BIT)) + 1] & \
        (1UL << ((index) % (sizeof(unsigned long) * CHAR_BIT)))) != 0

#else
//pokud je definovany no check
    #define bitarray_getbit(jmeno_pole, index) \
        (jmeno_pole[((index)/(sizeof(unsigned long) * CHAR_BIT)) + 1] & \
        (1UL << ((index) % (sizeof(unsigned long) * CHAR_BIT)))) != 0

#endif //konec NO_CHECK

#endif //konec INLINE rozvetveni

#endif //konec BITARRAY_H
