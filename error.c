
// error.c
// Řešení IJC-DU1, příklad b), 21.3.2026
// Autor: Josef Hruska, FSI
// Přeloženo: gcc 14.2.0
// Popis: Modul pro tisk chybových hlášení (warning, error_exit) s variabilním počtem argumentů. Využívá hlavičku <stdarg.h>

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "error.h"

void warning(const char *fmt, ...){
    fprintf(stderr, "Warning: ");
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, "\n");
}

void error_exit(const char *fmt, ...){
    fprintf(stderr, "Error: ");
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, "\n");
    exit(1);
}


