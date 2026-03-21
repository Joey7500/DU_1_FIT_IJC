// test_b.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

// --- ANSI Escape sekvence pro barvy ---
#define COLOR_RED "\x1b[31m"
#define COLOR_GREEN "\x1b[32m"
#define COLOR_YELLOW "\x1b[33m"
#define COLOR_CYAN "\x1b[36m"
#define COLOR_RESET "\x1b[0m"
// --------------------------------------

// Definice struktury pro 50 unikátních testů (stavový automat)
typedef struct
{
    const char *description;
    const char *input;
    const char *expected;
} TestCase;

TestCase tests[] = {
    // Zakladni text (Testy 1-5)
    {"Prázdný string", "", ""},
    {"Obyčejný kód", "int x = 5;", "int x = 5;"},
    {"Kód na více řádků", "int a;\nint b;", "int a;\nint b;"},
    {"Znak lomítka bez poznámky (zlomek)", "int a = 5 / 2;", "int a = 5 / 2;"},
    {"Znak hvězdičky (násobek)", "int a = 5 * 2;", "int a = 5 * 2;"},

    // Blokove poznamky (Testy 6-15)
    {"Jednoduchá bloková poznámka", "/* poznamka */", " "},
    {"Bloková poznámka s kódem", "int x; /* text */ int y;", "int x;   int y;"},
    {"Neukončená bloková poznámka na prvním řádku", "/* radek1\nradek2 */", " "},
    {"Bloková poznámka na začátku", "/*A*/int x;", " int x;"},
    {"Dvě blokové poznámky za sebou", "/*A*//*B*/", "  "},
    {"Bloková poznámka obsahující hvězdičky", "/****/", " "},
    {"Bloková poznámka obsahující lomítka", "/* // */", " "},
    {"Více řádků uvnitř blokové poznámky", "/*\n*\n*/", " "},
    {"Prázdná bloková poznámka", "/**/", " "},
    {"Dvě lomítka uvnitř blokové poznámky", "/*//*/", " "},

    // Radkove poznamky (Testy 16-27)
    {"Jednoduchá řádková poznámka", "// poznamka\nint x;\n", "\nint x;\n"},
    {"Trojité lomítko", "///poznamka poznamka\nint x;\n", "\nint x;\n"},
    {"Řádková poznámka na konci", "int x; // text\n", "int x; \n"},
    {"Řádková pozn. obsahující blokovou poznámku", "// /* text */\n", "\n"},
    {"Řádková pozn. obsahující neukonč. blokovou poznámku", "// /* text\n", "\n"},
    {"Prázdná řádková poznámka", "//\n", "\n"},
    {"Dvě řádkové poznámky za sebou", "// A\n// B\n", "\n\n"},
    {"Řádková poznámka s hvězdičkami", "// ****\n", "\n"},
    {"Řádková poznámka zalomená zpětným lomítkem", "// text \\\npokracovani\nint x;\n", "\nint x;\n"},
    {"Řádková poznámka s backslashem a mezerou (neplatné zalomení v C)", "// text \\ \nint x;", "\nint x;"},
    {"Řádková poznámka s uvozovkami", "// \"text\"\n", "\n"},
    {"Řádková poznámka s apostrofy", "// \'A\'\n", "\n"},

    // Retezce literaly (Testy 28-37)
    {"Řetězec s řádkovou poznámkou", "char* s = \"// ne\";", "char* s = \"// ne\";"},
    {"Řetězec s blokovou poznámkou", "char* s = \"/* ne */\";", "char* s = \"/* ne */\";"},
    {"Řetězec s lomítkem", "char* s = \"/\";", "char* s = \"/\";"},
    {"Řetězec s hvězdičkou", "char* s = \"*\";", "char* s = \"*\";"},
    {"Prázdný řetězec", "char* s = \"\";", "char* s = \"\";"},
    {"Řetězec s escapovanou uvozovkou", "char* s = \"\\\" /* ne */\";", "char* s = \"\\\" /* ne */\";"},
    {"Řetězec s escapovaným lomítkem", "char* s = \"\\\\\" /* ano */;", "char* s = \"\\\\\"  ;"},
    {"Více řetězců", "\"A\" /* ano */ \"B\"", "\"A\"   \"B\""},
    {"Escapovaný backslash na konci", "\"\\\\\" /* ok */", "\"\\\\\"  "},
    {"Makro definice s řetězcem", "#define A \"//\"", "#define A \"//\""},

    // Znakove literaly (Testy 38-47)
    {"Znak lomítka", "char c = '/';", "char c = '/';"},
    {"Znak hvězdičky", "char c = '*';", "char c = '*';"},
    {"Znak uvozovky", "char c = '\"';", "char c = '\"';"},
    {"Znak backslashe", "char c = '\\\\';", "char c = '\\\\';"},
    {"Znak apostrofu", "char c = '\\'';", "char c = '\\'';"},
    {"Apostrof a za ním řádková pozn.", "char c = 'A'; // ne", "char c = 'A'; "},
    {"Apostrof a za ním bloková pozn.", "char c = 'A'; /* ne */", "char c = 'A';  "},
    {"Prazdný znak (syntaktická chyba, ale automat musí přejít)", "char c = '';", "char c = '';"},
    {"Znak escapovaného konce řádku", "char c = '\\n';", "char c = '\\n';"},
    {"Octalový znak", "char c = '\\040';", "char c = '\\040';"},

    // Zakerne kombinace (Testy 48-52)
    {"Bloková pozn. smazaná, řetězec zůstal", "/* \" */ int x = \"/*\";", "  int x = \"/*\";"},
    {"Začátek blokové v stringu", "printf(\"/*\"); */", "printf(\"/*\"); */"},
    {"Escapovaný apostrof a řetězec", "'\\'' \"/*\" //\n", "'\\'' \"/*\" \n"},
    {"Lomítko na konci kódu bez pokračování", "int x = 5 /\n", "int x = 5 /\n"},
    {"Dereference a poznámka", "x = *y /* pointer */;\n", "x = *y  ;\n"}};

bool run_test(int test_number)
{
    int total_tests = sizeof(tests) / sizeof(TestCase);
    char buffer[4096] = "\0";

    if (test_number > total_tests)
    {
        fprintf(stderr, COLOR_YELLOW "test_b: Nelze spustit tento test: %i" COLOR_RESET "\n", test_number);
        return false;
    }

    // Vytvoření dočasného vstupního souboru
    FILE *in = fopen("temp_in.c", "w");
    fputs(tests[test_number].input, in);
    fclose(in);

    // Volání programu zapsáním výstupu do temp_out.c
    system("./no-comment temp_in.c > temp_out.c 2> temp_err.txt");

    // Přečtení výstupu
    FILE *out = fopen("temp_out.c", "r");
    buffer[0] = '\0';
    if (out)
    {
        size_t bytes_read = fread(buffer, 1, sizeof(buffer) - 1, out);
        buffer[bytes_read] = '\0'; // ořez
        fclose(out);
    }

    // Pokud se neshodují, program ohlásí testovací fail
    if (strcmp(buffer, tests[test_number].expected) != 0)
    {
        fprintf(stderr, COLOR_RED "TEST FAIL (%d): %s" COLOR_RESET "\n", test_number + 1, tests[test_number].description);
        fprintf(stderr, COLOR_RED "Očekáváno: [%s]" COLOR_RESET "\n", tests[test_number].expected);
        fprintf(stderr, COLOR_RED "Získáno:   [%s]" COLOR_RESET "\n\n", buffer);
        return false;
    }
    printf(COLOR_GREEN "OK: Test #%d (%s) proběhl uspěšně!" COLOR_RESET "\n", test_number + 1, tests[test_number].description);
    return true;
}

bool run_no_comment_tests()
{
    bool all_tests_successfull = 1;
    int total_tests = sizeof(tests) / sizeof(TestCase);
    char buffer[4096] = "\0";

    for (int i = 0; i < total_tests; i++)
    {
        all_tests_successfull &= run_test(i);
    }
    if (all_tests_successfull) printf(COLOR_GREEN "OK: Všech %d testů automatu no-comment proběhlo úspěšně!" COLOR_RESET "\n", total_tests);
    else fprintf(stderr, COLOR_RED "ERROR: Některé testy automatu no-comment neproběhly správně." COLOR_RESET "\n");
}

// Test chybového modulu pomocí POSIX return kodů
bool test_error_module()
{
    // Využijeme kompilátor pro sestavení malého testu
    system("echo '#include \"error.h\"\nint main() { warning(\"W\"); return 0; }' > test_warn.c");
    system("gcc test_warn.c error.c -o test_warn >/dev/null 2>&1");
    int ret_w = system("./test_warn 2>/dev/null");
    if (ret_w != 0) {
        fprintf(stderr, COLOR_RED "ERROR: warning ukončuje program." COLOR_RESET "\n");
        return false; // warning neukončí proces chybou
    }

    system("echo '#include \"error.h\"\nint main() { error_exit(\"E\"); return 0; }' > test_err.c");
    system("gcc test_err.c error.c -o test_err >/dev/null 2>&1");
    int ret_e = WEXITSTATUS(system("./test_err 2>/dev/null"));
    if (ret_e != 1) {
        fprintf(stderr, COLOR_RED "ERROR: error_exit neukončuje program." COLOR_RESET "\n");
        return false; // error_exit ukončí proces statusem 1 podle zadání
    }

    printf(COLOR_GREEN "OK: Modul error.c funguje správně" COLOR_RESET "\n");
    return true;
}

int main(int argc, char *argv[])
{
    bool all_tests_successful = 1;
    printf(COLOR_YELLOW "=== Začátek testů pro část B ===" COLOR_RESET "\n");
    if (argc > 1)
    {
        for (int i = 1; i < argc; i++)
        {
            if (strcmp(argv[i], "err") == 0) all_tests_successful &= test_error_module();
            else
                all_tests_successful &= run_test(atoi(argv[i]) - 1);
        }
        if (all_tests_successful)
            printf(COLOR_GREEN "=== Všechny vyžádané testy části B proběhly úspěšně ===" COLOR_RESET "\n");
        return 0;
    }
    all_tests_successful &= run_no_comment_tests();
    all_tests_successful &= test_error_module();
    printf(COLOR_GREEN "=== Všechny testy části B proběhly úspěšně ===" COLOR_RESET "\n");

    // Uklid temp souboru
    system("rm -f temp_in.c temp_out.c temp_err.txt test_warn.c test_warn test_err.c test_err");
    return 0;
}
