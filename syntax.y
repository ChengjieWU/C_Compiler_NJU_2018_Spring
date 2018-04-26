%{
    #include <stdio.h>
    #include <string.h>
    #include "grammarTree.h"
    #include "lex.yy.c"

    extern bool lexicalError;
    bool syntaxError = false;

    struct Node* grammarTreeRoot;
%}

%locations
%union {
    int type_int;
    float type_float;
    double type_double;
    char type_string[32];
    struct Node* type_node_pointer;
}

%token <type_node_pointer> INT INT8 INT16
%token <type_node_pointer> FLOAT FLOATE
%token <type_node_pointer> ASSIGNOP
%token <type_node_pointer> RELOP
%token <type_node_pointer> PLUS MINUS STAR DIV
%token <type_node_pointer> AND OR NOT
%token <type_node_pointer> TYPE
%token <type_node_pointer> STRUCT
%token <type_node_pointer> LC RC LB RB LP RP
%token <type_node_pointer> SEMI COMMA
%token <type_node_pointer> ID
%token <type_node_pointer> RETURN
%token <type_node_pointer> IF ELSE
%token <type_node_pointer> WHILE
%token <type_node_pointer> DOT

%right COMMA
%right ASSIGNOP
%left  OR
%left  AND
%left  RELOP
%left  PLUS MINUS
%left  STAR  DIV
%right NOT
%left  LP RP LB RB DOT 

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%type <type_node_pointer> Program
%type <type_node_pointer> ExtDefList ExtDef
%type <type_node_pointer> ExtDecList
%type <type_node_pointer> Specifier StructSpecifier OptTag Tag
%type <type_node_pointer> VarDec FunDec VarList ParamDec
%type <type_node_pointer> CompSt StmtList Stmt
%type <type_node_pointer> DefList Def DecList Dec Exp Args

%%
Program:    ExtDefList {
                $$ = create_node("Program\0", $1->line, false);
                create_link($$, $1);
                grammarTreeRoot = $$;
            }
        ;
ExtDefList: {
                $$ = create_node("ExtDefList\0", yylineno, false);
            }
        |   ExtDef ExtDefList {
                $$ = create_node("ExtDefList\0", $1->line, false);
                create_link($$, $1);
                create_link($$, $2);
            }
        ;
ExtDef:     Specifier ExtDecList SEMI {
                $$ = create_node("ExtDef\0", $1->line, false);
                create_link($$, $1);
                create_link($$, $2);
                create_link($$, $3);
            }
        |   Specifier SEMI {
                $$ = create_node("ExtDef\0", $1->line, false);
                create_link($$, $1);
                create_link($$, $2);
            }
        |   Specifier FunDec CompSt {
                $$ = create_node("ExtDef\0", $1->line, false);
                create_link($$, $1);
                create_link($$, $2);
                create_link($$, $3);
            }
        |   Specifier error {
                $$ = create_node("ExtDef\0", $1->line, false);
                printf("Error type B at Line %d: Missing \";\".\n", yylineno);
                create_link($$, $1);
            }
        |   Specifier ExtDecList error {
                $$ = create_node("ExtDef\0", $1->line, false);
                printf("Error type B at Line %d: Missing \";\".\n", yylineno);
                create_link($$, $1);
                create_link($$, $2);
            }
        ;
ExtDecList: VarDec {
                $$ = create_node("ExtDecList\0", $1->line, false);
                create_link($$, $1);
            }
        |   VarDec COMMA ExtDecList {
                $$ = create_node("ExtDecList\0", $1->line, false);
                create_link($$, $1);
                create_link($$, $2);
                create_link($$, $3);
            }
        ;
Specifier:  TYPE {
                $$ = create_node("Specifier\0", $1->line, false);
                create_link($$, $1);
            }
        |   StructSpecifier {
                $$ = create_node("Specifier\0", $1->line, false);
                create_link($$, $1);
            }
        ;
StructSpecifier:    STRUCT OptTag LC DefList RC {
                        $$ = create_node("StructSpecifier\0", $1->line, false);
                        create_link($$, $1);
                        create_link($$, $2);
                        create_link($$, $3);
                        create_link($$, $4);
                        create_link($$, $5);
                    }
                |   STRUCT Tag {
                        $$ = create_node("StructSpecifier\0", $1->line, false);
                        create_link($$, $1);
                        create_link($$, $2);
                    }
                ;
OptTag: {
            $$ = create_node("OptTag\0", yylineno, false);
        }
    |   ID {
            $$ = create_node("OptTag\0", $1->line, false);
            create_link($$, $1);
        }
    ;
Tag:    ID {
            $$ = create_node("Tag\0", $1->line, false);
            create_link($$, $1);
        }
    ;
VarDec: ID {
            $$ = create_node("VarDec\0", $1->line, false);
            create_link($$, $1);
        }
    |   VarDec LB INT RB {
            $$ = create_node("VarDec\0", $1->line, false);
            create_link($$, $1);
            create_link($$, $2);
            create_link($$, $3);
            create_link($$, $4);
        }
    |   VarDec LB INT error RB {
            $$ = create_node("VarDec\0", $1->line, false);
            create_link($$, $1);
            create_link($$, $2);
            create_link($$, $3);
            printf("Error type B at Line %d: Missing \"]\".\n", yylineno);
        }
    ;
FunDec: ID LP VarList RP {
            $$ = create_node("FunDec\0", $1->line, false);
            create_link($$, $1);
            create_link($$, $2);
            create_link($$, $3);
            create_link($$, $4);
        }
    |   ID LP RP {
            $$ = create_node("FunDec\0", $1->line, false);
            create_link($$, $1);
            create_link($$, $2);
            create_link($$, $3);
        }
    ;
VarList:ParamDec COMMA VarList {
            $$ = create_node("VarList\0", $1->line, false);
            create_link($$, $1);
            create_link($$, $2);
            create_link($$, $3);
        }
    |   ParamDec {
            $$ = create_node("VarList\0", $1->line, false);
            create_link($$, $1);
        }
    ;
ParamDec:Specifier VarDec {
            $$ = create_node("ParamDec\0", $1->line, false);
            create_link($$, $1);
            create_link($$, $2);
        }
    ;
CompSt: LC DefList StmtList RC {
            $$ = create_node("CompSt\0", $1->line, false);
            create_link($$, $1);
            create_link($$, $2);
            create_link($$, $3);
            create_link($$, $4);
        }
    |   error RC {
            $$ = create_node("CompSt\0", yylineno, false);
            create_link($$, $2);
            printf("Error type B at Line %d: Wrong function statements.\n", yylineno);
        }
    ;
StmtList:{
            $$ = create_node("StmtList\0", yylineno, false);
        }
    |   Stmt StmtList {
            $$ = create_node("StmtList\0", $1->line, false);
            create_link($$, $1);
            create_link($$, $2);
        }
    ;
Stmt:   Exp SEMI {
            $$ = create_node("Stmt\0", $1->line, false);
            create_link($$, $1);
            create_link($$, $2);
        }
    |   CompSt {
            $$ = create_node("Stmt\0", $1->line, false);
            create_link($$, $1);
        }
    |   RETURN Exp SEMI {
            $$ = create_node("Stmt\0", $1->line, false);
            create_link($$, $1);
            create_link($$, $2);
            create_link($$, $3);
        }
    |   IF LP Exp RP Stmt %prec LOWER_THAN_ELSE {
            $$ = create_node("Stmt\0", $1->line, false);
            create_link($$, $1);
            create_link($$, $2);
            create_link($$, $3);
            create_link($$, $4);
            create_link($$, $5);
        }
    |   IF LP Exp RP Stmt ELSE Stmt {
            $$ = create_node("Stmt\0", $1->line, false);
            create_link($$, $1);
            create_link($$, $2);
            create_link($$, $3);
            create_link($$, $4);
            create_link($$, $5);
            create_link($$, $6);
            create_link($$, $7);
        }
    |   WHILE LP Exp RP Stmt {
            $$ = create_node("Stmt\0", $1->line, false);
            create_link($$, $1);
            create_link($$, $2);
            create_link($$, $3);
            create_link($$, $4);
            create_link($$, $5);
        }
    |   error SEMI {
            $$ = create_node("Stmt\0", yylineno, false);
            create_link($$, $2);
            printf("Error type B at Line %d: Invalid statement.\n", yylineno);
        }
    |   Exp error {
            $$ = create_node("Stmt\0", $1->line, false);
            create_link($$, $1);
            printf("Error type B at Line %d: Missing \";\".\n", yylineno);
        }
    ;
DefList:{
            $$ = create_node("Deflist\0", yylineno, false);
        }
    |   Def DefList {
            $$ = create_node("DefList\0", $1->line, false);
            create_link($$, $1);
            create_link($$, $2);
        }
    ;
Def:    Specifier DecList SEMI {
            $$ = create_node("Def\0", $1->line, false);
            create_link($$, $1);
            create_link($$, $2);
            create_link($$, $3);
        }
    |   Specifier DecList error {
            $$ = create_node("Def\0", $1->line, false);
            create_link($$, $1);
            create_link($$, $2);
            printf("Error type B at Line %d: Missing \";\".\n", yylineno);
        }
    ;
DecList:Dec {
            $$ = create_node("DecList\0", $1->line, false);
            create_link($$, $1);
        }
    |   Dec COMMA DecList {
            $$ = create_node("DecList\0", $1->line, false);
            create_link($$, $1);
            create_link($$, $2);
            create_link($$, $3);
        }
    ;
Dec:    VarDec {
            $$ = create_node("Dec\0", $1->line, false);
            create_link($$, $1);
        }
    |   VarDec ASSIGNOP Exp {
            $$ = create_node("Dec\0", $1->line, false);
            create_link($$, $1);
            create_link($$, $2);
            create_link($$, $3);
        }
    ;
Exp:    Exp ASSIGNOP Exp {
            $$ = create_node("Exp\0", $1->line, false);
            create_link($$, $1);
            create_link($$, $2);
            create_link($$, $3);
        }
    |   Exp AND Exp {
            $$ = create_node("Exp\0", $1->line, false);
            create_link($$, $1);
            create_link($$, $2);
            create_link($$, $3);
        }
    |   Exp OR Exp {
            $$ = create_node("Exp\0", $1->line, false);
            create_link($$, $1);
            create_link($$, $2);
            create_link($$, $3);
        }
    |   Exp RELOP Exp {
            $$ = create_node("Exp\0", $1->line, false);
            create_link($$, $1);
            create_link($$, $2);
            create_link($$, $3);
        }
    |   Exp PLUS Exp {
            $$ = create_node("Exp\0", $1->line, false);
            create_link($$, $1);
            create_link($$, $2);
            create_link($$, $3);
        }
    |   Exp MINUS Exp {
            $$ = create_node("Exp\0", $1->line, false);
            create_link($$, $1);
            create_link($$, $2);
            create_link($$, $3);
        }
    |   Exp STAR Exp {
            $$ = create_node("Exp\0", $1->line, false);
            create_link($$, $1);
            create_link($$, $2);
            create_link($$, $3);
        }
    |   Exp DIV Exp {
            $$ = create_node("Exp\0", $1->line, false);
            create_link($$, $1);
            create_link($$, $2);
            create_link($$, $3);
        }
    |   LP Exp RP {
            $$ = create_node("Exp\0", $1->line, false);
            create_link($$, $1);
            create_link($$, $2);
            create_link($$, $3);
        }
    |   MINUS Exp {
            $$ = create_node("Exp\0", $1->line, false);
            create_link($$, $1);
            create_link($$, $2);
        }
    |   NOT Exp {
            $$ = create_node("Exp\0", $1->line, false);
            create_link($$, $1);
            create_link($$, $2);
        }
    |   ID LP Args RP {
            $$ = create_node("Exp\0", $1->line, false);
            create_link($$, $1);
            create_link($$, $2);
            create_link($$, $3);
            create_link($$, $4);
        }
    |   ID LP RP {
            $$ = create_node("Exp\0", $1->line, false);
            create_link($$, $1);
            create_link($$, $2);
            create_link($$, $3);
        }
    |   Exp LB Exp RB {
            $$ = create_node("Exp\0", $1->line, false);
            create_link($$, $1);
            create_link($$, $2);
            create_link($$, $3);
            create_link($$, $4);
        }
    |   Exp DOT ID {
            $$ = create_node("Exp\0", $1->line, false);
            create_link($$, $1);
            create_link($$, $2);
            create_link($$, $3);
        }
    |   ID {
            $$ = create_node("Exp\0", $1->line, false);
            create_link($$, $1);
        }
    |   INT {
            $$ = create_node("Exp\0", $1->line, false);
            create_link($$, $1);
        }
    |   FLOAT {
            $$ = create_node("Exp\0", $1->line, false);
            create_link($$, $1);
        }
    ;
Args:   Exp COMMA Args {
            $$ = create_node("Args\0", $1->line, false);
            create_link($$, $1);
            create_link($$, $2);
            create_link($$, $3);
        }
    |   Exp {
            $$ = create_node("Args\0", $1->line, false);
            create_link($$, $1);
        }
    ;
%%



yyerror (char* msg)
{   
    syntaxError = true;
    //printf("Syntax error\n");
    return;
}
