%{
    #include <stdio.h>
%}

%union {
    int type_int;
    float type_float;
    double type_double;
}

%token <type_int> INT
%token <type_float> FLOAT
%token PLUS MINUS STAR DIV

%type <type_double> Exp Factor Term

%%
Calc:
    |   Exp { printf("= %lf\n", $1); }
    ;
Exp:    Factor
    |   Exp PLUS Factor { $$ = $1 + $3; printf("%d, %d, %d\n", @1.first_line), @1.first_column, @1.last_column}
    |   Exp MINUS Factor { $$ = $1 - $3; }
    ;
Factor: Term
    |   Factor STAR Term { $$ = $1 * $3; }
    |   Factor DIV Term { $$ = $1 / $3; }
    ;
Term:   INT { $$ = $1; }
    |   FLOAT { $$ = $1; }
    ;

%%

#include "lex.yy.c"

yyerror (char* msg)
{
    fprintf(stderr, "error: %s\n", msg);
}
