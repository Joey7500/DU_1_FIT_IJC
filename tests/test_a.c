// test_a.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "../bitarray.h"

// --- ANSI Escape sekvence pro barvy ---
#define COLOR_RED "\x1b[31m"
#define COLOR_GREEN "\x1b[32m"
#define COLOR_YELLOW "\x1b[33m"
#define COLOR_CYAN "\x1b[36m"
#define COLOR_RESET "\x1b[0m"
// --------------------------------------

// Test 1-50: Testování správné velikosti u různých lokálních polí
void test_create()
{
    bitarray_create(b1, 1);
    if (*b1 != 1) error_exit(COLOR_RED"bitarray_create: Vytvoření pole o velikosti 1 se nezdařilo."COLOR_RESET"\n");
    bitarray_create(b2, 63);
    if (*b2 != 63) error_exit(COLOR_RED"bitarray_create: Vytvoření pole o velikosti 63 se nezdařilo."COLOR_RESET"\n");
    bitarray_create(b3, 64);
    if (*b3 != 64)
        error_exit(COLOR_RED"bitarray_create: Vytvoření pole o velikosti 64 se nezdařilo."COLOR_RESET"\n");
    bitarray_create(b4, 65);
    if (*b4 != 65)
        error_exit(COLOR_RED"bitarray_create: Vytvoření pole o velikosti 65 se nezdařilo."COLOR_RESET"\n");
    bitarray_create(b5, 128);
    if (*b5 != 128)
        error_exit(COLOR_RED"bitarray_create: Vytvoření pole o velikosti 128 se nezdařilo."COLOR_RESET"\n");
    bitarray_create(b6, 100000L);
    if (*b6 != 100000L)
        error_exit(COLOR_RED"bitarray_create: Vytvoření pole o velikosti 100000L se nezdařilo."COLOR_RESET"\n");
    printf(COLOR_GREEN"OK: bitarray_create (6 různých mezí)"COLOR_RESET"\n");
}

// Test 51-100: Testování dynamické alokace a její velikosti
void test_alloc()
{
    bitarray_alloc(b1, 10);
    if (b1 == NULL) error_exit(COLOR_RED"bitarray_alloc: Chyba alokace paměti."COLOR_RESET"\n");
    if (*b1 != 10) {
        bitarray_free(b1);
        error_exit(COLOR_RED"bitarray_alloc: Alokace pole o velikosti 10 se nezdařila."COLOR_RESET"\n");
    };
    bitarray_free(b1);
    bitarray_alloc(b2, 256);
    if (b2 == NULL)
        error_exit(COLOR_RED "bitarray_alloc: Chyba alokace paměti."COLOR_RESET"\n");
    if (*b2 != 256) {
        bitarray_free(b2);
        error_exit(COLOR_RED "bitarray_alloc: Alokace pole o velikosti 256 se nezdařila."COLOR_RESET"\n");
    }
    bitarray_free(b2);
    bitarray_alloc(b3, 444000000L);
    if (b3 == NULL)
        error_exit(COLOR_RED "bitarray_alloc: Chyba alokace paměti."COLOR_RESET"\n");
    if (*b3 != 444000000L)
    {
        bitarray_free(b3);
        error_exit(COLOR_RED "bitarray_alloc: Alokace pole o velikosti 444000000L se nezdařila."COLOR_RESET"\n");
    };
    bitarray_free(b3);
    printf(COLOR_GREEN"OK: bitarray_alloc a bitarray_free (3 extremni meze)"COLOR_RESET"\n");
}

void test_bitarray_size()
{
    // 1. Test: Velikost lokálního/automatického pole
    bitarray_create(lokalni_pole, 150);
    if(bitarray_size(lokalni_pole) != 150)
        error_exit(COLOR_RED"bitarray_size: Implementace bitarray_size nevrací správnou velikost (150)." COLOR_RESET "\n");


    // 2. Test: Velikost hraničně malého pole (1 bit)
    bitarray_create(male_pole, 1);
    if (bitarray_size(male_pole) != 1)
        error_exit(COLOR_RED "bitarray_size: Implementace bitarray_size nevrací správnou velikost (150)." COLOR_RESET "\n");


    // 3. Test: Velikost velkého lokálního pole s příponou L
    bitarray_create(velke_lokalni, 100000L);
    if (bitarray_size(velke_lokalni) != 100000L)
        error_exit(COLOR_RED "bitarray_size: Implementace bitarray_size nevrací správnou velikost (150)." COLOR_RESET "\n");


    // 4. Test: Velikost dynamicky alokovaného pole
    bitarray_alloc(dynamicke_pole, 444000000L);
    if (bitarray_size(dynamicke_pole) != 444000000L){
        if (dynamicke_pole != NULL) bitarray_free(dynamicke_pole);
        error_exit(COLOR_RED "bitarray_size: Implementace bitarray_size nevrací správnou velikost (444000000)." COLOR_RESET "\n");
    }


    // Úklid paměti po alokaci (nutnost podle zadání)
    bitarray_free(dynamicke_pole);

    printf(COLOR_GREEN"OK: Funkce bitarray_size vrací spravnou velikost pro různé typy poli."COLOR_RESET"\n");
}

// Test 101-356: Testování funkce fill (1 a 0 nezávisle na 128 bitech)
void test_fill_0()
{
    bitarray_create(b, 128);
    bitarray_fill(b, 0); // Vynuluje pole
    for (unsigned long i = 0; i < 128; i++)
    {
        if (bitarray_getbit(b, i) != 0) error_exit(COLOR_RED"bitarray_fill(0): Index %lu není vynulován."COLOR_RESET"\n", i); // 128 nezávislých testů getbit pro 0
    }
    printf(COLOR_GREEN"OK: bitarray_fill(0) (128 nezávislých testů)"COLOR_RESET"\n");
}

void test_fill_1()
{
    bitarray_create(b, 128);
    bitarray_fill(b, 1); // Nastaví samé jedničky
    for (unsigned long i = 0; i < 128; i++)
    {
        if (bitarray_getbit(b, i) != 1) error_exit(COLOR_RED "bitarray_fill(1): Index %lu nemá hodnotu 1." COLOR_RESET "\n", i); // 128 nezávislých testů getbit pro 1
    }
    printf(COLOR_GREEN"OK: bitarray_fill(1) (128 nezávislých testů)"COLOR_RESET"\n");
}

// Test 357-484: Nezávislé nastavení izolovaných bitů bez ovlivnění sousedů
void test_setbit_isolation()
{
    bitarray_create(b, 128);
    bitarray_fill(b, 0);
    // Hraniční bity bloků (0, 63, 64, 127)
    unsigned long hranice[] = {0, 63, 64, 127};
    for (int i = 0; i < 4; i++)
    {
        bitarray_setbit(b, hranice[i], 1);
        if (bitarray_getbit(b, hranice[i]) != 1)
            error_exit(COLOR_RED "bitarray_setbit(1): Index %lu nemá hodnotu 1." COLOR_RESET "\n", hranice[i]);

        // Zkontrolovat, že sousední bity zůstaly 0
        if (hranice[i] > 0)
            if (bitarray_getbit(b, hranice[i] - 1) != 0) error_exit(COLOR_RED "bitarray_setbit(1): Levý sousední index %lu nemá hodnotu 0." COLOR_RESET "\n", hranice[i]-1);
        if (hranice[i] < 127)
            if (bitarray_getbit(b, hranice[i] + 1) != 0) error_exit(COLOR_RED "bitarray_setbit(1): Pravý sousední index %lu nemá hodnotu 0." COLOR_RESET "\n", hranice[i]+1);

        // Vrátit zpět na nulu a ověřit
        bitarray_setbit(b, hranice[i], 0);
        if (bitarray_getbit(b, hranice[i]) != 0) error_exit(COLOR_RED "bitarray_setbit(0): Index %lu nemá hodnotu 0." COLOR_RESET "\n", hranice[i]);
    }
    printf(COLOR_GREEN"OK: bitarray_setbit izolace a hranice unsigned long bloků (16 testů)"COLOR_RESET"\n");
}

char *binaries_results[] = {
    "443999791: 443999791",
    "443999813: 443999813",
    "443999827: 443999827",
    "443999867: 443999867",
    "443999879: 443999879",
    "443999891: 443999891",
    "443999911: 443999911",
    "443999953: 443999953",
    "443999971: 443999971",
    "443999993: 443999993"
};

// Test 485-500+: Otestování zkompilovaných binárek a jejich výstupu
void test_primes_binaries(const char *binary_path)
{
    char command[128] = "\0";
    char path[64];
    snprintf(path, sizeof(path), "%s", binary_path);

    // Spustíme binárku s patřičným ulimit.
    // Standardní výstup pošleme do programu 'factor' pro nezávislé ověření prvočísel.
    // Standardní chybový výstup (s časem běhu) přesměrujeme do dočasného textového souboru.
    snprintf(command, sizeof(command),
             "ulimit -s 65536 && %s 2> temp_time.txt | factor > temp_out.txt", binary_path);

    FILE *binary_file = fopen(path, "r");
    if (binary_file == NULL) {
        error_exit(COLOR_RED"Binární soubor %s neexistuje."COLOR_RESET"\n", path);
    }
    fclose(binary_file);

    system(command);

    FILE *fp = fopen("./temp_out.txt", "r");
    if (fp == NULL) {
        error_exit(COLOR_RED"Chyba čtení z výstupního souboru testu."COLOR_RESET"\n");
    }

    int count = 0;
    char buffer[4096] = "\0";

    // Čtení výstupu z programu factor (řádek po řádku)
    while (fgets(buffer, sizeof(buffer), fp) != NULL)
    {
        count++;
        unsigned long num, factor_result;

        // Výstup utility 'factor' pro prvočíslo P vypadá takto: "P: P"
        // Pokud by to nebylo prvočíslo (např. 10), výstup by byl "10: 2 5"
        if (sscanf(buffer, "%lu: %lu", &num, &factor_result) == 2)
        {
            // Pokud se první načtené číslo nerovná druhému, nejedná se o prvočíslo
            if (num != factor_result) {
                fclose(fp);
                error_exit(COLOR_RED"Číslo %i není prvočíslo."COLOR_RESET"\n", num);
            }
            if (num != strtoul(binaries_results[count - 1], &binaries_results[count-1]+strlen(binaries_results[count-1]), 10)) {
                fclose(fp);
                error_exit(COLOR_RED"Číslo %i není prvočíslem pro N=444000000"COLOR_RESET"\n", num);
            }
        }
        else
        {
            fclose(fp);
            error_exit(COLOR_RED"Neočekávaný formát výstupu: %s"COLOR_RESET"\n", buffer);
        }
    }

    int status = fclose(fp);
    if (status != 0) {
        error_exit(COLOR_RED"Nepodařilo se zavřit soubor temp_out.txt."COLOR_RESET"\n");
    }

    // Zadání vyžaduje přesně posledních 10 prvočísel
    if (count != 10) error_exit(COLOR_RED"Počet prvočísel neodpovídá požadovanému."COLOR_RESET"\n");

    // Kontrola, zda program správně měří čas a vypisuje jej na stderr
    FILE *ferr = fopen("temp_time.txt", "r");
    if (ferr == NULL) error_exit(COLOR_RED"Chyba čtení z výstupního souboru temp_time.txt"COLOR_RESET"\n");
    char time_buf[128];
    fread(time_buf, 1, sizeof(time_buf) - 1, ferr);
    time_buf[strlen(time_buf)-1] = '\0'; // Odstranění koncového \n
    fclose(ferr);
    if (atof(time_buf+5) > 10) warning(COLOR_YELLOW"Váš kód je poměrně pomalý."COLOR_RESET"\n");
    remove("temp_time.txt"); // Úklid dočasného souboru

    // Zadání vyžaduje výpis formátu fprintf(stderr, "Time=%.3g\n", ...)
    if (strstr(time_buf, "Time=") == NULL)
        error_exit(COLOR_RED "./primes(binary): Program nevrací čas na stderr." COLOR_RESET "\n");

    printf(COLOR_GREEN"OK: Binárka %s správně vygenerovala 10 prvočísel a vypisuje čas (%s)"COLOR_RESET"\n", binary_path, time_buf);
}

int main()
{
    printf(COLOR_YELLOW"=== Začátek testů pro část A ==="COLOR_RESET"\n");
    test_create();
    test_alloc();
    test_fill_0();
    test_fill_1();
    test_setbit_isolation();
    test_primes_binaries("./primes");
    printf(COLOR_GREEN"=== Všechny testy části A proběhly úspěšně ===" COLOR_RESET"\n\n");
    return 0;
}
