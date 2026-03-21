// error.h
// Řešení IJC-DU1, příklad b), 21.3.2026
// Autor: Josef Hruska, FSI
// Přeloženo: gcc 14.2.0
// Popis: Rozhraní chybového modulu pro tisk varování a ukončení programu.

#ifndef ERROR_H
#define ERROR_H

void warning(const char *fmt, ...);
void error_exit(const char *fmt, ...);

#endif
