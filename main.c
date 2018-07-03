#include <stdio.h>
#include "semantic.h"
#include "translate.h"
#include "codeGeneration.h"

/* ------ IMPORTANT NOTICE ------ */
/* change LAB to 1, 2, 3, 4 to check experiment output of 4 labs */
#define LAB 4
/* ------------------------------ */

extern int yylineno;
//extern int yydebug;
extern bool lexicalError;
extern bool syntaxError;
extern bool syntaxErrorPrinted;
extern int errorLine;
extern int generalErrorLine;

void initialize_error_status()
{
    lexicalError = false;
    syntaxError = false;
    syntaxErrorPrinted = false;
    errorLine = -1;
    generalErrorLine = -1;
}

#if LAB == 4

int main(int argc, char** argv)
{
    if (argc != 3) {
        return -1;
    }
    FILE* f = fopen(argv[1], "r");
    if (!f) {
        perror(argv[1]);
        return -1;
    }
    initialize_error_status();
    yyrestart(f);
    yylineno = 1;
    yyparse();
    fclose(f);
    if (!lexicalError && !syntaxError) {
        semantic_analysis();
        CB ic = translate_IR();
        f = fopen(argv[2], "w");
        if (!f) {
            perror(argv[2]);
            return -1;
        }
        generate_code(ic, f);
        // write_CB(ic, f);
        fclose(f);
    }
    return 0;
}

#elif LAB == 3

int main(int argc, char** argv)
{
    if (argc != 3) {
        return -1;
    }
    FILE* f = fopen(argv[1], "r");
    if (!f) {
        perror(argv[1]);
        return -1;
    }
    initialize_error_status();
    yyrestart(f);
    yylineno = 1;
    yyparse();
    fclose(f);
    if (!lexicalError && !syntaxError) {
        f = fopen(argv[2], "w");
        if (!f) {
            perror(argv[2]);
            return -1;
        }
        write_CB(translate_IR(), f);
        fclose(f);
    }
    return 0;
}

#elif LAB == 2

int main(int argc, char** argv)
{
    if (argc < 2) {
        yylex();
        return 0;
    }
    int i = 0;
    for (i = 1; i < argc; i++) {
        FILE* f = fopen(argv[i], "r");
        if (!f) {
            perror(argv[i]);
            return 1;
        }
        initialize_error_status();
        yyrestart(f);
        yylineno = 1;
        yyparse();
        fclose(f);
        if (!lexicalError && !syntaxError) {
            // print_tree(grammarTreeRoot, 0);
        }
        else if (!lexicalError && syntaxError && !syntaxErrorPrinted) {
            printf("Error type B at Line %d: Syntax error.\n", generalErrorLine);
        }
        if (!lexicalError && !syntaxError) {
            semantic_analysis();
            // printSymbols();
        }
    }
    return 0;
}

#elif LAB == 1

int main(int argc, char** argv)
{
    if (argc < 2) {
        yylex();
        return 0;
    }
    int i = 0;
    for (i = 1; i < argc; i++) {
        FILE* f = fopen(argv[i], "r");
        if (!f) {
            perror(argv[i]);
            return 1;
        }
        initialize_error_status();
        yyrestart(f);
        yylineno = 1;
        //yydebug = 1;
        yyparse();
        fclose(f);
        if (!lexicalError && !syntaxError) {
            print_tree(grammarTreeRoot, 0);
        }
    }
    return 0;
}

#endif