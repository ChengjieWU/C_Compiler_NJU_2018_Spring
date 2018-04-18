%{
    #include <stdio.h>
%}

%option yylineno

DIGIT       [0-9]
LETTER      [_a-zA-Z]
DL          [_a-zA-Z0-9]

INT         0|[1-9][0-9]*
INTW        [1-9]{DL}*{LETTER}{DL}*
INT8        0(0|[1-7][0-7]*)
INT8W       0[89a-wyzA-WYZ_]+{DL}*|00{DL}+|0[1-7]{DL}*[89a-zA-Z_]{DL}*
INT16       0[xX](0|[1-9A-Fa-f][0-9A-Fa-f]*)
INT16W      (0[xX](0{DL}+|{DL}*[G-Zg-z_]{DL}*))|0[xX]
FLOAT       {INT}\.{DIGIT}*|\.{DIGIT}+
FLOATE      {FLOAT}e[+-]?{INT}
FLOATEW     {FLOAT}e
ID          {LETTER}{DL}*
SEMI        ;
COMMA       ,
ASSIGNOP    =
RELOP       <|>|<=|>=|==|!=
PLUS        \+
MINUS       -
STAR        \*
DIV         \/
AND         &&
OR          \|\|
DOT         \.
NOT         !
TYPE        int|float
LP          \(
RP          \)
LB          \[
RB          \]
LC          \{
RC          \}
STRUCT      struct
RETURN      return
IF          if
ELSE        else
WHILE       while


%%

{INT}  {
    printf("INT at line %d: \'%s\'\n", yylineno, yytext);
}
{INTW} {
    printf("INTW at line %d: \'%s\'\n", yylineno, yytext);
}
{INT8} {
    printf("INT8 at line %d: \'%s\'\n", yylineno, yytext);
}
{INT8W} {
    printf("INT8W at line %d: \'%s\'\n", yylineno, yytext);
}
{INT16} {
    printf("INT16 at line %d: \'%s\'\n", yylineno, yytext);
}
{INT16W} {
    printf("INT16W at line %d: \'%s\'\n", yylineno, yytext);
}
{FLOAT} {
    printf("FLOAT at line %d: \'%s\'\n", yylineno, yytext);
}
{FLOATE} {
    printf("FLOATE at line %d: \'%s\'\n", yylineno, yytext);
}
{FLOATEW} {
    printf("FLOATEW at line %d: \'%s\'\n", yylineno, yytext);
}

{SEMI} {
    printf("RELOP at line %d: \'%s\'\n", yylineno, yytext);
}
{COMMA} {
    printf("RELOP at line %d: \'%s\'\n", yylineno, yytext);
}
{ASSIGNOP} {
    printf("RELOP at line %d: \'%s\'\n", yylineno, yytext);
}
{RELOP} {
    printf("RELOP at line %d: \'%s\'\n", yylineno, yytext);
}
{PLUS} {
    printf("RELOP at line %d: \'%s\'\n", yylineno, yytext);
}
{MINUS} {
    printf("RELOP at line %d: \'%s\'\n", yylineno, yytext);
}
{STAR} {
    printf("RELOP at line %d: \'%s\'\n", yylineno, yytext);
}
{DIV} {
    printf("RELOP at line %d: \'%s\'\n", yylineno, yytext);
}
{AND} {
    printf("RELOP at line %d: \'%s\'\n", yylineno, yytext);
}
{OR} {
    printf("RELOP at line %d: \'%s\'\n", yylineno, yytext);
}
{DOT} {
    printf("RELOP at line %d: \'%s\'\n", yylineno, yytext);
}
{NOT} {
    printf("RELOP at line %d: \'%s\'\n", yylineno, yytext);
}
{TYPE} {
    printf("RELOP at line %d: \'%s\'\n", yylineno, yytext);
}
{LP} {
    printf("RELOP at line %d: \'%s\'\n", yylineno, yytext);
}
{RP} {
    printf("RELOP at line %d: \'%s\'\n", yylineno, yytext);
}
{LB} {
    printf("RELOP at line %d: \'%s\'\n", yylineno, yytext);
}
{RB} {
    printf("RELOP at line %d: \'%s\'\n", yylineno, yytext);
}
{LC} {
    printf("RELOP at line %d: \'%s\'\n", yylineno, yytext);
}
{RC} {
    printf("RELOP at line %d: \'%s\'\n", yylineno, yytext);
}
{STRUCT} {
    printf("RELOP at line %d: \'%s\'\n", yylineno, yytext);
}
{RETURN} {
    printf("RELOP at line %d: \'%s\'\n", yylineno, yytext);
}
{IF} {
    printf("RELOP at line %d: \'%s\'\n", yylineno, yytext);
}
{ELSE} {
    printf("RELOP at line %d: \'%s\'\n", yylineno, yytext);
}
{WHILE} {
    printf("RELOP at line %d: \'%s\'\n", yylineno, yytext);
}
{ID} {
    printf("ID at line %d: \'%s\'\n", yylineno, yytext);
}
\n {}
. {
    printf("Mysterious character at line %d: \'%s\'\n", yylineno, yytext);
}
