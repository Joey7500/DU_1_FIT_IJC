// eratosthenes.c
// Řešení IJC-DU1, příklad a), 21.3.2026
// Autor: Josef Hruska, FSI
// Přeloženo: gcc 14.2.0
// Popis: Samotna implementace algoritmu Eratosthenova sita
// Omezení: Pro velká lokální pole vyžaduje zvětšení limitu zásobníku (ulimit -s).

#include <stdio.h>
#include "bitarray.h"
#include <math.h>


void Eratosthenes(bitarray_t pole){
    unsigned long velikost_N = bitarray_size(pole);
    bitarray_fill(pole, 1);
    bitarray_setbit(pole, 0, 0);
    bitarray_setbit(pole, 1, 0);
    unsigned long limit = sqrt(velikost_N);
    for (unsigned long i = 2; i <= limit; i++){
        if (bitarray_getbit(pole, i)) {
            for(unsigned long j = i*i; j < velikost_N; j += i){
                bitarray_setbit(pole, j, 0);
            }
        }
    }
}

