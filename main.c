#include <stdio.h>
#include "grammarTree.h"

extern int yylineno;
//extern int yydebug;
extern bool lexicalError;
extern bool syntaxError;
extern bool syntaxErrorPrinted;
extern int errorLine;
extern int generalErrorLine;
extern struct Node* grammarTreeRoot;

void initialize_error_status()
{
    lexicalError = false;
    syntaxError = false;
    syntaxErrorPrinted = false;
    errorLine = -1;
    generalErrorLine = -1;
}

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
        if (!lexicalError && !syntaxError) print_tree(grammarTreeRoot, 0);
        else if (syntaxError && !syntaxErrorPrinted) {
            printf("Error type B at Line %d: Syntax error.\n", generalErrorLine);
        }
        fclose(f);
    }
    return 0;
}