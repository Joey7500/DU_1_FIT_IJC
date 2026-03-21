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
            error_exit("Chyba: Soubor %s se nepodarilo otevrit.\n", argv[1]);
        }
        struct stat stat_vstup, stat_vystup;
        fstat(fileno(vstup),&stat_vstup);
        fstat(fileno(stdout), &stat_vystup);
        
        if(stat_vstup.st_ino == stat_vystup.st_ino && stat_vstup.st_dev == stat_vystup.st_dev ) {
            error_exit("Chyba: Soubor vstupni a vystupni soubor se shoduji");
        }

        int c;
        stav_automatu_t aktualni_stav = NORMAL;

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

                case LINE_COMMENT: if (c =='\n' ) {aktualni_stav = NORMAL; putchar('\n'); }
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

                case STRING: if (c == '\') {aktualni_stav = STRING_ESCAPE; putchar(c) ;}
                else if (c == "\"") {aktualni_stav = NORMAL; putchar(c); }
                else {aktualni_stav = STRING; putchar(c); }
                break;

                case STRING_ESCAPE: if ()
                break;

                case CHAR:
                break;

                case CHAR_ESCAPE:
                break;



                
                }
            }
        }
    }
    return 0;
}
