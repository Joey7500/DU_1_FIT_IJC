#include <stdio.h>
#include "bitarray.h"
#include <time.h>

#ifdef USE_INLINE
extern inline unsigned long bitarray_size(bitarray_t jmeno_pole);
extern inline void bitarray_fill(bitarray_t jmeno_pole, int bool_vyraz);
extern inline void bitarray_setbit(bitarray_t jmeno_pole, bitarray_index_t index, int bool_vyraz);
extern inline int bitarray_getbit(bitarray_t jmeno_pole, bitarray_index_t index);
#endif

void Eratosthenes(bitarray_t pole);

#define HRANICE 444000000
int main() {
    clock_t start = clock();
    bitarray_create(pole,HRANICE);
    Eratosthenes(pole);
    unsigned long nalezena_prvocisla[10];
    int pocet = 0;
    
    for(unsigned long i = HRANICE-1; i>0;i--) {
        if(bitarray_getbit(pole,i)) {
            nalezena_prvocisla[pocet] = i;
            pocet++;
        }
        if(pocet == 10){
            break;
        }
    }

    for (int j = 9; j >= 0; j--){
        printf("%lu\n", nalezena_prvocisla[j]);

    }
    fprintf(stderr, "Time=%.3g\n", (double)(clock()-start)/CLOCKS_PER_SEC);
    return 0;
}

