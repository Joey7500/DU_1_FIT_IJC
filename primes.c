#include <stdio.h>
#include "bitarray.h"
#include <time.h>

void Eratosthenes(bitarray_t pole);


int main() {
    clock_t start = clock();
    bitarray_create(pole, 100);
    Eratosthenes(pole);
    unsigned long nalezena_prvocisla[10];
    int pocet = 0;
    
    for(unsigned long i = 99; i>0;i--) {
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

