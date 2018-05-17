#ifndef __SYMBOL_H__
#define __SYMBOL_H__

#include "type.h"

#define SYMBOL_NAME_MAX_LEN 32

enum symbolKind {VARIABLE, FUNCTION, STRUCT};

struct Symbol {
    enum symbolKind kind;
    char name[SYMBOL_NAME_MAX_LEN];
    union {
        struct Type *structure;
    };
    struct Symbol *next;
};

struct Symbol *rootHead;

void initializeSymbol();
void printSymbols();
int general_check(char *name);
int add_structure(char *name, struct Type *stc);
struct Type *search_structure(char *name);
int add_variable(char *name, struct Type *stc);

#endif