#include <stdio.h>

extern int yylineno;
//extern int yydebug;

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
        yyrestart(f);
        yylineno = 1;
        //yydebug = 1;
        yyparse();
        fclose(f);
    }
    return 0;
}