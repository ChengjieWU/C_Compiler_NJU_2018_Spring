#include <mm_malloc.h>
#include <string.h>
#include "symbol.h"

void initializeSymbol()
{
    rootHead = NULL;
}

int general_check(char *name)
{
    struct Symbol *p = rootHead;
    for (; p != NULL; p = p->next) {
        if (strcmp(p->name, name) == 0) {
            return 0;
        }
    }
    return -1;
}

int add_structure(char *name, struct Type *stc)
{
    if (!general_check(name)) return -1;
    struct Symbol *sym = (struct Symbol *)malloc(sizeof(struct Symbol));
    sym->next = rootHead;
    sym->structure = stc;
    strcpy(sym->name, name);
    rootHead = sym;
    return 0;
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