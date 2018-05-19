#ifndef __SYMBOL_H__
#define __SYMBOL_H__

#include "type.h"

#define SYMBOL_NAME_MAX_LEN 32

enum symbolKind {VARIABLE, FUNCTION, STRUCT};

struct Symbol {
    enum symbolKind kind;               // 符号类型
    char name[SYMBOL_NAME_MAX_LEN];     // 符号标识符
    struct Type *structure;             // 表示数据类型。在FUNCTION中表示返回值类型
    struct Symbol *params;              // 维护函数定义的参数表中的指针
    struct Symbol *next;                // 维护符号表中的指针
};

struct Symbol *rootHead;

void initializeSymbol();
void printSymbols();
int general_check(char *name);
struct Symbol *add_structure(char *name, struct Type *stc);
struct Type *search_structure(char *name);
struct Symbol *add_variable(char *name, struct Type *stc);
struct Symbol *add_function(char *name, struct Type *retType, struct Symbol *params);
struct Symbol *search_variable(char *name);
struct Symbol *search_function(char *name);
struct Symbol *search_symbol(char *name);

#endif