#include <mm_malloc.h>
#include <string.h>
#include <stdio.h>
#include "symbol.h"

void initializeSymbol()
{
    rootHead = NULL;
    struct Symbol *sym = add_function("read", typeInt, NULL);
    sym->kind = FUNC_READ;
    sym = add_function("write", typeInt, NULL);
    sym->kind = FUNC_WRITE;
}

void printSymbols()
{
    struct Symbol *p = rootHead;
    for (; p != NULL; p = p->next) {
        switch (p->kind) {
            case VARIABLE:
                printf("Variable: %s\t", p->name);
                print_type(p->structure);
                printf("\n");
                break;
            case STRUCT:
                printf("Structure: %s\t", p->name);
                print_type(p->structure);
                printf("\n");
                break;
            case FUNCTION:
                printf("Function: %s\t", p->name);
                print_type(p->structure);
                struct Symbol *q = p->params;
                for (; q != NULL; q = q->params) {
                    printf("%s\t", q->name);
                }
                printf("\n");
            default:
                break;
        }
    }
}

int general_check(char *name)
{
    struct Symbol *p = rootHead;
    for (; p != NULL; p = p->next) {
        if (strcmp(p->name, name) == 0) {
            return -1;
        }
    }
    return 0;
}

struct Symbol *add_structure(char *name, struct Type *stc)
{
    if (general_check(name) == -1) return NULL;
    struct Symbol *sym = (struct Symbol *)malloc(sizeof(struct Symbol));
    sym->kind = STRUCT;
    sym->next = rootHead;
    strcpy(sym->name, name);
    sym->structure = stc;
    rootHead = sym;
    return sym;
}

struct Symbol *add_variable(char *name, struct Type *stc)
{
    if (general_check(name) == -1) return NULL;
    struct Symbol *sym = (struct Symbol *)malloc(sizeof(struct Symbol));
    sym->kind = VARIABLE;
    sym->next = rootHead;
    strcpy(sym->name, name);
    sym->structure = stc;
    rootHead = sym;
    return sym;
}

struct Symbol *add_function(char *name, struct Type *retType, struct Symbol *params)
{
    if (general_check(name) == -1) return NULL;
    struct Symbol *sym = (struct Symbol *)malloc(sizeof(struct Symbol));
    sym->kind = FUNCTION;
    sym->next = rootHead;
    strcpy(sym->name, name);
    sym->structure = retType;
    sym->params = params;
    rootHead = sym;
    return sym;
}

struct Type *search_structure(char *name)
{
    struct Symbol *p = rootHead;
    for (; p != NULL; p = p->next) {
        if (p->kind == STRUCT) {
            if (strcmp(p->name, name) == 0) {
                return p->structure;
            }
        }
    }
    return NULL;
}

struct Symbol *search_variable(char *name)
{
    struct Symbol *p = rootHead;
    for (; p != NULL; p = p->next) {
        if (p->kind == VARIABLE) {
            if (strcmp(p->name, name) == 0) {
                return p;
            }
        }
    }
    return NULL;
}

struct Symbol *search_function(char *name)
{
    struct Symbol *p = rootHead;
    for (; p != NULL; p = p->next) {
        if (p->kind == FUNCTION) {
            if (strcmp(p->name, name) == 0) {
                return p;
            }
        }
    }
    return NULL;
}

struct Symbol *search_symbol(char *name)
{
    struct Symbol *p = rootHead;
    for (; p != NULL; p = p->next) {
        if (strcmp(p->name, name) == 0) {
            return p;
        }
    }
    return NULL;
}
