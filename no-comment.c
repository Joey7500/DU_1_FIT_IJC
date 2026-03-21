// no-comment.c
// Řešení IJC-DU1, příklad b), 21.3.2026
// Autor: Josef Hruska, FSI
// Přeloženo: gcc 14.2.0
// Popis: Stavovy automat pro odstranovani komentaru ze zdrojoveho kódu jazyka C.
// Poznamky: Předpoklada syntakticky korektni vstupni kod. Pro detekci presmerovani souboru jsou pouzity POSIX funkce fstat a fileno (vyzaduje makro _POSIX_C_SOURCE).

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "error.h"

typedef enum {
    NORMAL,
    SLASH,
    LINE_COMMENT,
    LINE_COMMENT_ESCAPE,
    BLOCK_COMMENT,
    BLOCK_COMMENT_STAR,
    STRING,
    STRING_ESCAPE,
    CHAR,
    CHAR_ESCAPE,
} stav_automatu_t;


int main(int argc, char *argv[]) {
    FILE *vstup = stdin;
    if (argc > 1) {
        vstup = fopen(argv[1] , "r");
            if (vstup == NULL) {
                error_exit("Chyba: Soubor %s se nepodarilo otevrit.\n", argv[1]); //snad ho smaze
            }
            struct stat stat_vstup, stat_vystup;
            fstat(fileno(vstup),&stat_vstup);
            fstat(fileno(stdout), &stat_vystup);
        
            if(stat_vstup.st_ino == stat_vystup.st_ino && stat_vstup.st_dev == stat_vystup.st_dev ) {
                error_exit("Chyba: Soubor vstupni a vystupni soubor se shoduji");
            }
        }
        int c;
        stav_automatu_t aktualni_stav = NORMAL; /*doufam ze smaze i tento*/

        while ((c = fgetc(vstup)) != EOF) {
            switch(aktualni_stav) {

                case NORMAL: if (c == '/') {aktualni_stav = SLASH; }
                else if (c == '\'') {aktualni_stav = CHAR; putchar(c); }
                else if (c == '\"') {aktualni_stav = STRING; putchar(c); }
                else {putchar(c); }
                break;
                
                case SLASH: if (c == '/') {aktualni_stav = LINE_COMMENT; }
                else if (c == '*') {aktualni_stav = BLOCK_COMMENT; }
                else {aktualni_stav = NORMAL; putchar('/'); putchar(c); }
                break;

                case LINE_COMMENT: if (c =='\n' ) {aktualni_stav = NORMAL; putchar(c); }
                else if (c == '\\') {aktualni_stav = LINE_COMMENT_ESCAPE; }
                else {aktualni_stav = LINE_COMMENT; }
                break;

                case LINE_COMMENT_ESCAPE: aktualni_stav = LINE_COMMENT;
                break;

                case BLOCK_COMMENT: if (c == '*') {aktualni_stav = BLOCK_COMMENT_STAR; }
                else {aktualni_stav = BLOCK_COMMENT; }
                break;

                case BLOCK_COMMENT_STAR: if (c == '*'){aktualni_stav = BLOCK_COMMENT_STAR; }
                else if (c == '/') {aktualni_stav = NORMAL; putchar(' '); }
                else {aktualni_stav = BLOCK_COMMENT; }
                break;

                case STRING: if (c == '\\') {aktualni_stav = STRING_ESCAPE; putchar(c) ;}
                else if (c == '\"') {aktualni_stav = NORMAL; putchar(c); }
                else {aktualni_stav = STRING; putchar(c); }
                break;

                case STRING_ESCAPE: {aktualni_stav = STRING; putchar(c); }
                break;

                case CHAR: if (c == '\\') {aktualni_stav = CHAR_ESCAPE; putchar(c); }
                else if (c == '\'') {aktualni_stav = NORMAL; putchar(c); }
                else {aktualni_stav = CHAR; putchar(c); }
                break;

                case CHAR_ESCAPE: {aktualni_stav = CHAR; putchar(c); }
                break;                
                }
            }
        
        if (aktualni_stav == SLASH) {
        putchar('/');
        aktualni_stav = NORMAL;
        }

        if (aktualni_stav != NORMAL && aktualni_stav != LINE_COMMENT) {
        error_exit("Chyba: Neukoncena poznamka nebo retezec na konci souboru.\n");
        }
    
    return 0;
}
