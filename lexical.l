%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "syntax.tab.h"
    #include "grammarTree.h"

    int yycolumn = 1;
    #define YY_USER_ACTION \
        yylloc.first_line = yylloc.last_line = yylineno; \
        yylloc.first_column = yycolumn; \
        yylloc.last_column = yycolumn + yyleng - 1; \
        yycolumn += yyleng;
%}

%option yylineno

DIGIT       [0-9]
LETTER      [_a-zA-Z]
DL          [_a-zA-Z0-9]
ANNOTATION  \/\*(.|\n)*\*\/
ANNOTATIONW \/\*(.|\n)*\/\*(.|\n)*\*\/(.|\n)*\*\/

INT         0|[1-9][0-9]*
INTW        [1-9]{DL}*{LETTER}{DL}*
INT8        0(0|[1-7][0-7]*)
INT8W       0[89a-wyzA-WYZ_]+{DL}*|00{DL}+|0[1-7]{DL}*[89a-zA-Z_]{DL}*
INT16       0[xX](0|[1-9A-Fa-f][0-9A-Fa-f]*)
INT16W      (0[xX](0{DL}+|{DL}*[G-Zg-z_]{DL}*))|0[xX]
FLOAT       {INT}\.{DIGIT}*|\.{DIGIT}+
FLOATE      ({DIGIT}+\.{DIGIT}*|\.{DIGIT}+)[eE][+-]?{DIGIT}+
FLOATEW     ({DIGIT}+\.{DIGIT}*|\.{DIGIT}+)[eE][+-]?
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
{ANNOTATIONW} {
    printf("Wrong annotation\n");
}
{ANNOTATION} {
}
{INT}  {
    yylval.type_node_pointer = create_node("INT\0", yylineno, true);
    yylval.type_node_pointer->type_int = atoi(yytext);
    return INT;
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
    yylval.type_node_pointer = create_node("FLOAT\0", yylineno, true);
    yylval.type_node_pointer->type_float = atof(yytext);
    return FLOAT;
}
{FLOATE} {
    printf("FLOATE at line %d: \'%s\'\n", yylineno, yytext);
}
{FLOATEW} {
    printf("FLOATEW at line %d: \'%s\'\n", yylineno, yytext);
}

{SEMI} {
    yylval.type_node_pointer = create_node("SEMI\0", yylineno, true);
    return SEMI;
}
{COMMA} {
    yylval.type_node_pointer = create_node("COMMA\0", yylineno, true);
    return COMMA;
}
{ASSIGNOP} {
    yylval.type_node_pointer = create_node("ASSIGNOP\0", yylineno, true);
    return ASSIGNOP;
}
{RELOP} {
    yylval.type_node_pointer = create_node("RELOP\0", yylineno, true);
    strcpy(yylval.type_node_pointer->type_string, yytext);
    return RELOP;
}
{PLUS} {
    yylval.type_node_pointer = create_node("PLUS\0", yylineno, true);
    return PLUS;
}
{MINUS} {
    yylval.type_node_pointer = create_node("MINUS\0", yylineno, true);
    return MINUS;
}
{STAR} {
    yylval.type_node_pointer = create_node("STAR\0", yylineno, true);
    return STAR;
}
{DIV} {
    yylval.type_node_pointer = create_node("DIV\0", yylineno, true);
    return DIV;
}
{AND} {
    yylval.type_node_pointer = create_node("AND\0", yylineno, true);
    return AND;
}
{OR} {
    yylval.type_node_pointer = create_node("OR\0", yylineno, true);
    return OR;
}
{DOT} {
    yylval.type_node_pointer = create_node("DOT\0", yylineno, true);
    return DOT;
}
{NOT} {
    yylval.type_node_pointer = create_node("NOT\0", yylineno, true);
    return NOT;
}
{TYPE} {
    yylval.type_node_pointer = create_node("TYPE\0", yylineno, true);
    strcpy(yylval.type_node_pointer->type_string, yytext);
    return TYPE;
}
{LP} {
    yylval.type_node_pointer = create_node("LP\0", yylineno, true);
    return LP;
}
{RP} {
    yylval.type_node_pointer = create_node("RP\0", yylineno, true);
    return RP;
}
{LB} {
    yylval.type_node_pointer = create_node("LB\0", yylineno, true);
    return LB;
}
{RB} {
    yylval.type_node_pointer = create_node("RB\0", yylineno, true);
    return RB;
}
{LC} {
    yylval.type_node_pointer = create_node("LC\0", yylineno, true);
    return LC;
}
{RC} {
    yylval.type_node_pointer = create_node("RC\0", yylineno, true);
    return RC;
}
{STRUCT} {
    yylval.type_node_pointer = create_node("STRUCT\0", yylineno, true);
    return STRUCT;
}
{RETURN} {
    yylval.type_node_pointer = create_node("RETURN\0", yylineno, true);
    return RETURN;
}
{IF} {
    yylval.type_node_pointer = create_node("IF\0", yylineno, true);
    return IF;
}
{ELSE} {
    yylval.type_node_pointer = create_node("ELSE\0", yylineno, true);
    return ELSE;
}
{WHILE} {
    yylval.type_node_pointer = create_node("WHILE\0", yylineno, true);
    return WHILE;
}
{ID} {
    yylval.type_node_pointer = create_node("ID\0", yylineno, true);
    strcpy(yylval.type_node_pointer->type_string, yytext);
    return ID;
}
" "+ {
}
\n {
    yycolumn = 1;
}
. {
    printf("Error type A at Line %d: Mysterious characters \'%s\'\n", yylineno, yytext);
}